#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 binormal;
layout(location = 4) in vec2 uv;

layout(location = 5) in vec3 model_position;
layout(location = 6) in vec4 model_orientation;
layout(location = 7) in vec3 model_scale;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;

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

    matrix[1][0] = s.y * (2 * (oxy - owx));
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
    mat4 transform = Transform(model_position, model_orientation, model_scale);
    gl_Position = u_proj_matrix *
                  u_view_matrix *
                  transform * vec4(position, 1.0);
}