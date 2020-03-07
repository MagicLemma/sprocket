#version 400 core

out vec4 out_Colour;

in vec2 blueTextureCoords[11];

uniform sampler2D originalTexture;

void main() {
    out_Colour = vec4(0.0);
    out_Colour += texture(originalTexture, blueTextureCoords[0]) * 0.0093;
    out_Colour += texture(originalTexture, blueTextureCoords[1]) * 0.028002;
    out_Colour += texture(originalTexture, blueTextureCoords[2]) * 0.065984;
    out_Colour += texture(originalTexture, blueTextureCoords[3]) * 0.121703;
    out_Colour += texture(originalTexture, blueTextureCoords[4]) * 0.175713;
    out_Colour += texture(originalTexture, blueTextureCoords[5]) * 0.198596;
    out_Colour += texture(originalTexture, blueTextureCoords[6]) * 0.175713;
    out_Colour += texture(originalTexture, blueTextureCoords[7]) * 0.121703;
    out_Colour += texture(originalTexture, blueTextureCoords[8]) * 0.065984;
    out_Colour += texture(originalTexture, blueTextureCoords[9]) * 0.028002;
    out_Colour += texture(originalTexture, blueTextureCoords[10]) * 0.0093;
}