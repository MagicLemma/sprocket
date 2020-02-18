#version 400 core

in vec3 d_colour;

layout(location = 0) out vec4 out_Colour;

void main()
{
    out_Colour = vec4(d_colour, 1.0);
}