#version 400 core

in vec2 textureCoords;

out vec4 out_Colour;

uniform sampler2D colourTexture;

void main(void){

	out_Colour = vec4(vec3(1.0) - texture(colourTexture, textureCoords).rgb, 1.0);

}