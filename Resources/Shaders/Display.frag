#version 400 core

layout(location = 0) out vec4 out_Colour;

uniform vec3 colour;

void main()
{
    out_Colour = vec4(colour.xyz, 1.0);
}