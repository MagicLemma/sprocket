#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture;

out vec2 d_texture;
out vec3 surfaceNormal;
out vec3 toLightVectors[5];
out vec3 toCameraVector;

uniform mat4 transformMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightPositions[5];

void main()
{
    vec4 worldPosition = transformMatrix * vec4(position, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
    d_texture = texture;

    surfaceNormal = (transformMatrix * vec4(normal, 0.0)).xyz;
    toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

    for (int i = 0; i != 5; i++) {
        toLightVectors[i] = lightPositions[i] - worldPosition.xyz;
    }
}