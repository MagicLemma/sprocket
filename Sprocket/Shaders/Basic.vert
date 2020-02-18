#version 400 core

layout(location = 0) in vec4 position;

out vec3 colour;

void main()
{
    gl_Position = position;
    colour = vec3(position.x + 0.5, 1.0, position.y + 0.5);
}