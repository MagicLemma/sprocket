#version 400 core
layout (location = 0) in vec3 position;

out vec2 blueTextureCoords[11];

uniform int target_height;

void main(void){
	gl_Position = vec4(position, 1.0);
	vec2 centerTexCoords = position.xy * 0.5 + 0.5;
    float pixelSize = 1.0 / target_height;

    for (int i = -5; i <= 5; i++) {
        blueTextureCoords[i + 5] = centerTexCoords + vec2(0.0, pixelSize * i);
    }
}