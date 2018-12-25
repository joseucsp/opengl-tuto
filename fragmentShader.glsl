#version 320 es

precision mediump float;

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

// Out data
out vec3 color;

void main()
{
    //color = vec3(0.7,0.0,0);
    color = fragmentColor;
}