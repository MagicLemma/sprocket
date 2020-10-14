#version 450 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 texture_coords;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

layout(location = 5) in ivec4 bone_indices;
layout(location = 6) in vec4 bone_weights;

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

uniform mat4 u_bone_transforms[50];

void main()
{
    vec4 total_position = vec4(0.0);
    vec4 total_normal = vec4(0.0);
    for (int i = 0; i < 4; i++) {
        if (bone_indices[i] > -1) {
            int index = bone_indices[i];
            float weight = bone_weights[i];

            mat4 transform = u_bone_transforms[index];
            transform = mat4(1.0);

            vec4 pos = transform * vec4(in_position, 1.0);
            total_position += pos * weight;

            vec4 nor = transform * vec4(in_normal, 0.0);
            total_normal += nor * weight;
        }
    }

    vec3 position = total_position.xyz;
    vec3 normal = total_normal.xyz;

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