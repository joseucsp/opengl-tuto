#include "common/loadshader.hpp"

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,  
};

int main(int argc, char** argv)
{
    if (!glfwInit()){
        std::cout << "Initialization failed" <<std::endl; 
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

    if (!window){
        std::cout << "Window or OpenGL context creation failer!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    // Dark blue background
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    

    GLuint VertexArrayID;
    glGenBuffers(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders( "vertexShader.glsl", 
                                    "fragmentShader.glsl" );

	// Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View  = glm::lookAt(  glm::vec3(0,0,3), // Camera is at (4,3,3), in World Space
								    glm::vec3(0,0,0), // and looks at the origin
								    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                 );

	// Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    
    glm::mat4 ScalingMatrix   = glm::scale(glm::mat4(1.0f),glm::vec3(0.8f, 0.8f , 0.8f));
    Model = ScalingMatrix * Model;
    
    
    glm::vec3 RotationAxis(0.0f, 0.0f, 1.0f);
    glm::mat4 RotateMatrix = glm::rotate(Model, (glm::mediump_float)M_PI * 1.0f, RotationAxis );

    Model = RotateMatrix * Model;
    
    glm::mat4 TranslateMatrix = glm::translate( Model, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = TranslateMatrix * Model;
    
    
	// Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around    
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifiers in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give out vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                                         g_vertex_buffer_data ,
                                         GL_STATIC_DRAW       );



    do {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,          // attibute 0, must match the layout in the shader
            3,          // size
            GL_FLOAT,   // type
            GL_FALSE,   // normalizerd?
            0,          // stride
            (void*)0    // array buffer offset
        );

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw the triangle
        // Starting from vertex 0; 3 vertices total -> 1 triangle

        glDrawArrays(GL_TRIANGLES, 0, 3);         
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
          glfwWindowShouldClose(window)       == 0            );
    

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    
    glfwTerminate();
    
    return 0;
}
