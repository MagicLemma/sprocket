#version 400 core

layout(location = 0) out vec4 out_Colour;

uniform vec3 colour;
uniform sampler2D tex;

in vec2 pass_texCoords;

void main()
{
    out_Colour = vec4(colour, 1.0);
}