#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coords;

out vec2  p_texture_coords;
out vec3  p_surface_normal;
out vec3  p_to_camera_vector;
out vec3  p_to_light_vector[5];

uniform mat4 u_model_matrix;
uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;
uniform vec3 u_light_pos[5];

void main()
{
    vec4 world_pos = u_model_matrix * vec4(position, 1.0);
    gl_Position = u_proj_matrix * u_view_matrix * world_pos;
    
    p_texture_coords = texture_coords;
    p_surface_normal = (u_model_matrix * vec4(normal, 0.0)).xyz;
    p_to_camera_vector = (inverse(u_view_matrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - world_pos.xyz;

    for (int i = 0; i != 5; i++) {
        p_to_light_vector[i] = u_light_pos[i] - world_pos.xyz;
    }
}