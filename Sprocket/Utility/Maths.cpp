#include "Maths.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace Maths {

mat4 transform(const vec3& translation, const vec3& rotation, const vec3& scale)
{
    mat4 matrix(1.0);
    matrix = glm::translate(matrix, translation);
    matrix = glm::rotate(matrix, radians(rotation.x), vec3(1, 0, 0));
    matrix = glm::rotate(matrix, radians(rotation.y), vec3(0, 1, 0));
    matrix = glm::rotate(matrix, radians(rotation.z), vec3(0, 0, 1));
    matrix = glm::scale(matrix, scale);
    return matrix;
}

mat4 transform(const vec3& translation, const vec3& rotation, float scale)
{
    return transform(translation, rotation, {scale, scale, scale});
}

mat4 transform(const vec3& translation, const vec3& rotation)
{
    return transform(translation, rotation, 1.0f);
}

mat4 translate(const mat4& matrix, const vec3& translation)
{
    return glm::translate(matrix, translation);
}

mat4 rotate(const mat4& matrix, const vec3& axis, float radians)
{
    return glm::rotate(matrix, radians, axis);
}

mat4 scale(const mat4& matrix, const vec3& scales)
{
    return glm::scale(matrix, scales);
}

mat4 scale(const mat4& matrix, float scale)
{
    return glm::scale(matrix, {scale, scale, scale});
}

mat4 perspective(float aspectRatio,
                 float fov,
                 float nearPlane,
                 float farPlane)
{
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);  
}

mat4 createViewMatrix(const vec3& position,
                      float pitch,
                      float yaw,
                      float roll)
{
    mat4 matrix(1.0);
    matrix = glm::rotate(matrix, radians(pitch), vec3(1, 0, 0));
    matrix = glm::rotate(matrix, radians(yaw), vec3(0, 1, 0));
    matrix = glm::rotate(matrix, radians(roll), vec3(0, 0, 1));
    matrix = glm::translate(matrix, -position);
    return matrix;
}

mat4 lookAt(const vec3& position,
            const vec3& target,
            const vec3& up)
{
    return glm::lookAt(position, target, up);
}

mat4 ortho(float left, float right, float bottom, float top)
{
    return glm::ortho(left, right, bottom, top);   
}

mat4 inverse(const mat4& matrix)
{
    return glm::inverse(matrix);
}

mat4 transpose(const mat4& matrix)
{
    return glm::transpose(matrix);
}

float radians(float degrees)
{
    return glm::radians(degrees);
}

float degrees(float radians)
{
    return glm::degrees(radians);
}

float sind(float degrees)
{
    return std::sin(radians(degrees));
}

float cosd(float degrees)
{
    return std::cos(radians(degrees));
}

void clamp(float& value, float min, float max)
{
    value = std::min(std::max(value, min), max);
}

vec3 cross(const vec3& lhs, const vec3& rhs)
{
    return glm::cross(lhs, rhs);
}

void normalise(vec3& vec)
{
    vec = glm::normalize(vec);
}

float distance(const Maths::vec2& A, const Maths::vec2& B)
{
    return glm::distance(A, B);
}

vec4 toQuaternion(const vec3& euler)
{
    double yaw = Maths::radians(euler.z);
    double pitch = Maths::radians(euler.y);
    double roll = Maths::radians(euler.x);

    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    double x = sr * cp * cy - cr * sp * sy;
    double y = cr * sp * cy + sr * cp * sy;
    double z = cr * cp * sy - sr * sp * cy;
    double w = cr * cp * cy + sr * sp * sy;

    return {x, y, z, w};
}

vec3 toEuler(const vec4& q)
{
    double z = -std::atan2(2 * (q.x * q.y + q.z * q.w), 1 - 2 * (q.y * q.y + q.z * q.z));
    double y = std::asin(2 * (q.x * q.z - q.w * q.y));
    double x = std::atan2(2 * (q.x * q.w + q.y * q.z), 1 - 2 * (q.z * q.z + q.w * q.w));
    return {x, y, z};
}

}
}