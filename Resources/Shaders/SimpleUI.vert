#version 410 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 colour;

uniform mat4 u_proj_matrix;

out vec3 pass_colour;

void main()
{
    pass_colour = colour;
    gl_Position = u_proj_matrix * vec4(position, 0, 1);
}