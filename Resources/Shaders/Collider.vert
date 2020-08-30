#version 400 core
layout(location = 0) in vec3 position;

uniform mat4 u_model_matrix;
uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;

void main()
{
    vec4 world_pos = u_model_matrix * vec4(position, 1.0);
    gl_Position = u_proj_matrix * u_view_matrix * world_pos;
}