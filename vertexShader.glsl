#version 320 es

/*
location = 0, refers to the buffer we use to feed the VertexPosition_modelspace
attibrute.

Each vertex can have numerous attributes : A position, one or several colours, 
one or several texture coordinates, lots of other things. 
OpenGL doesn’t know what a colour is : it just sees a vec3. 
So we have to tell him which buffer corresponds to which input. 
We do that by setting the layout to the same value as the first parameter 
to glVertexAttribPointer. The value “0” is not important, 
it could be 12 (but no more than glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &v) ), 
the important thing is that it’s the same number on both sides.

“vertexPosition_modelspace” could have any other name. It will contain
the position of the vertex for each run of the vertex shader.

“in” means that this is some input data. Soon we’ll see the “out” keyword.

*/

layout(location = 0) in vec3 vertexPosition_modelspace;

void main()
{
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w   = 1.0;

}