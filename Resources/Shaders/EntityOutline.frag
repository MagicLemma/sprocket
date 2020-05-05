#version 400 core

layout(location = 0) out vec4 out_colour;

uniform vec4 u_colour;

void main()
{
    out_colour = u_colour;
}