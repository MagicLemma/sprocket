#version 330 core
out vec4 out_Colour;

uniform sampler2D textureSampler;
uniform vec3 colour;

in vec2 pass_textureCoord;

void main()
{
    out_Colour = vec4(colour, texture(textureSampler, pass_textureCoord).a);
}