#version 400 core
layout (location = 0) in vec3 position;

out vec2 blueTextureCoords[11];

uniform float target_width;

void main(void){
	gl_Position = vec4(position, 1.0);
	vec2 centerTexCoords = position.xy * 0.5 + 0.5;
    float pixelSize = 1.0 / target_width;

    for (int i = -5; i <= 5; i++) {
        blueTextureCoords[i + 5] = centerTexCoords + vec2(pixelSize * i, 0.0);
    }
}