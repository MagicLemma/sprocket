#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texture_coords;

layout(location = 5) in vec3 model_position;
layout(location = 6) in vec4 model_orientation;
layout(location = 7) in vec3 model_scale;

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
uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_light_proj_view;

mat4 make_model_matrix(vec3 p, vec4 o, vec3 s)
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
    mat4 model_matrix = make_model_matrix(model_position, model_orientation, model_scale);
    vec4 world_pos = model_matrix * vec4(position, 1.0);
    gl_Position = u_proj_matrix * u_view_matrix * world_pos;
    
    p_data.world_position = vec3(world_pos);
    p_data.texture_coords = texture_coords;
    
    p_data.world_normal = mat3(model_matrix) * normal;
    p_data.world_tangent = mat3(model_matrix) * tangent;
    p_data.world_bitangent = mat3(model_matrix) * bitangent;

    p_data.normal = normal;
    p_data.tangent = tangent;
    p_data.bitangent = bitangent;

    p_data.tangent_space = mat3(model_matrix) * mat3(tangent, bitangent, normal);

    p_data.light_space_pos = u_light_proj_view * world_pos;
    p_data.to_camera = (inverse(u_view_matrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - world_pos.xyz;
}