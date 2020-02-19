#version 400 core

in vec2 d_texture;

layout(location = 0) out vec4 out_Colour;

uniform sampler2D textureSampler;

void main()
{
    out_Colour = texture(textureSampler, d_texture);
}