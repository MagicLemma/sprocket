#version 400 core

layout(location = 0) in vec3 position;

uniform mat4 transformMatrix;

void main()
{
    vec4 screenPosition = transformMatrix * vec4(position, 1.0);
    gl_Position = screenPosition;
}