#version 400 core

in vec3 textureCoords;
out vec4 out_Color;

uniform samplerCube cubeMap;

void main() {
    out_Color = texture(cubeMap, textureCoords);
}