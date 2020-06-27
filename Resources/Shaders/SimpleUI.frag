#version 410 core

in vec4 pass_colour;

layout (location = 0) out vec4 out_colour;

void main()
{
    out_colour = pass_colour;
}