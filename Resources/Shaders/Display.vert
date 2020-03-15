#version 400 core

layout(location = 0) in vec2 position;

uniform mat4 projection;
uniform mat4 transform;

void main()
{
    gl_Position = projection * transform * vec4(position, 1.0, 1.0);
}