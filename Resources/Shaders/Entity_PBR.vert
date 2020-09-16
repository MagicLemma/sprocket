#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texture_coords;

layout(location = 5) in vec3 model_position;
layout(location = 6) in vec4 model_orientation;
layout(location = 7) in vec3 model_scale;

out vec2 p_texture_coords;
out vec3 p_surface_normal;
out vec3 p_to_camera_vector;
out vec3 p_to_light_vector[5];

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
} p_data;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;
uniform vec3 u_light_pos[5];

// Shadows
uniform mat4 u_light_proj_view;
out vec4 p_light_space_pos;

mat4 Transform(vec3 p, vec4 o, vec3 s)
{
    mat4 matrix;

    float oxx = o.x * o.x;
    float oyy = o.y * o.y;
    float ozz = o.z * o.z;
    float oxz = o.x * o.z;
    float oxy = o.x * o.y;
    float oyz = o.y * o.z;
    float owx = o.w * o.x;
    float owy = o.w * o.y;
    float owz = o.w * o.z;

    matrix[0][0] = s.x * (1 - 2 * (oyy + ozz));
    matrix[0][1] = s.x * (2 * (oxy + owz));
    matrix[0][2] = s.x * (2 * (oxz - owy));
    matrix[0][3] = 0;

    matrix[1][0] = s.y * (2 * (oxy - owz));
    matrix[1][1] = s.y * (1 - 2 * (oxx + ozz));
    matrix[1][2] = s.y * (2 * (oyz + owx));
    matrix[1][3] = 0;

    matrix[2][0] = s.z * (2 * (oxz + owy));
    matrix[2][1] = s.z * (2 * (oyz - owx));
    matrix[2][2] = s.z * (1 - 2 * (oxx + oyy));
    matrix[2][3] = 0;

    matrix[3][0] = p.x;
    matrix[3][1] = p.y;
    matrix[3][2] = p.z;
    matrix[3][3] = 1;

    return matrix;
}

void main()
{
    mat4 model_matrix = Transform(model_position, model_orientation, model_scale);
    vec4 world_pos = model_matrix * vec4(position, 1.0);
    gl_Position = u_proj_matrix * u_view_matrix * world_pos;
    
    p_data.world_position = vec3(world_pos);

    p_texture_coords = texture_coords;
    p_surface_normal = (model_matrix * vec4(normal, 0.0)).xyz;
    p_to_camera_vector = (inverse(u_view_matrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - world_pos.xyz;

    p_light_space_pos = u_light_proj_view * world_pos;

    for (int i = 0; i != 5; i++) {
        p_to_light_vector[i] = u_light_pos[i] - world_pos.xyz;
    }
}