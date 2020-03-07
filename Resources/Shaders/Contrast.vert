#version 400 core

in vec2 position;
in vec2 texCoords;

out vec2 textureCoords;

void main(void){

	gl_Position = vec4(position, 0.0, 1.0);
	textureCoords = texCoords;
	
}