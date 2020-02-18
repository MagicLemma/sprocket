#version 400 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 colour;

out vec3 d_colour;

void main()
{
    gl_Position = position;
    d_colour = colour;
}