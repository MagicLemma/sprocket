#version 410 core

in vec4 pass_colour;
in vec2 pass_textureCoords;

uniform sampler2D tex;

layout (location = 0) out vec4 out_colour;

void main()
{
    out_colour = pass_colour * texture(tex, pass_textureCoords);
}