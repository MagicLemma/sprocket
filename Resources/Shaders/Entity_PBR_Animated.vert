#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

layout(location = 5) in ivec4 x;
layout(location = 6) in vec4 y;

out Data
{
    vec3 world_position;
    vec2 texture_coords;

    vec3 world_normal;
    vec3 world_tangent;
    vec3 world_bitangent;

    vec3 normal;
    vec3 tangent;
    vec3 bitangent;

    mat3 tangent_space;

    vec4 light_space_pos;
    vec3 to_camera;
} p_data;

// Transforms
uniform mat4 u_model_matrix;
uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_light_proj_view;

void main()
{
    vec4 world_pos = u_model_matrix * vec4(position, 1.0);
    gl_Position = u_proj_matrix * u_view_matrix * world_pos;
    
    p_data.world_position = vec3(world_pos);
    p_data.texture_coords = texture_coords;
    
    p_data.world_normal = mat3(u_model_matrix) * normal;
    p_data.world_tangent = mat3(u_model_matrix) * tangent;
    p_data.world_bitangent = mat3(u_model_matrix) * bitangent;

    p_data.normal = normal;
    p_data.tangent = tangent;
    p_data.bitangent = bitangent;

    p_data.tangent_space = mat3(u_model_matrix) * mat3(tangent, bitangent, normal);

    p_data.light_space_pos = u_light_proj_view * world_pos;
    p_data.to_camera = (inverse(u_view_matrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - world_pos.xyz;
}