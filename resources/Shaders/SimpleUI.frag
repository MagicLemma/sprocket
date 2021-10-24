#version 410 core

in vec4 pass_colour;
in vec2 pass_textureCoords;

uniform sampler2D tex;

uniform int texture_channels;

layout (location = 0) out vec4 out_colour;

void main()
{
    if (texture_channels > 1) {
        out_colour = pass_colour * texture(tex, pass_textureCoords);
    }
    else {
        float a = texture(tex, pass_textureCoords).r;
        out_colour = vec4(pass_colour.rgb, pass_colour.a * a);
    }
}