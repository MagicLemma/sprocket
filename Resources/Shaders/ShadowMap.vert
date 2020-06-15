#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 u_model_matrix;
uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;

void main()
{
    gl_Position = u_proj_matrix *
                  u_view_matrix *
                  u_model_matrix * vec4(position, 1.0);
}