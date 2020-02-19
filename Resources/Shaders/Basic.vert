#version 400 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture;

out vec2 d_texture;

uniform mat4 transformMatrix;

void main()
{
    gl_Position = transformMatrix * position;
    d_texture = texture;
}