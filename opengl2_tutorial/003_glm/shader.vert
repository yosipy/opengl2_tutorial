#version 120

//
// shader.vert
//

uniform mat4 MVP;

void main(void)
{
    gl_Position = MVP * gl_Vertex;
    gl_FrontColor = gl_Color;
}