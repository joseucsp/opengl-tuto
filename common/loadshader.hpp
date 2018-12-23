#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);