#version 410 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 textureCoords;

uniform mat4 u_proj_matrix;

out vec4 pass_colour;
out vec2 pass_textureCoords;

void main()
{
    pass_colour = colour;
    pass_textureCoords = textureCoords;
    gl_Position = u_proj_matrix * vec4(position, 0, 1);
}