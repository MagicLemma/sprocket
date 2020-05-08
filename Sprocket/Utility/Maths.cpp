#include "Maths.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace Maths {

mat4 transform(const vec3& position, const quat& orientation)
{
    mat4 m = toMat3(orientation);
    m[3][0] = position.x;
    m[3][1] = position.y;
    m[3][2] = position.z;
    m[3][3] = 1.0f;
    return m;
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

mat4 view(const vec3& position,
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

vec3 getTranslation(const mat4& transform)
{
    return vec3{transform[3][0], transform[3][1], transform[3][2]};
}

float magnitude(const vec3& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float magnitudeSquare(const vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

std::string toString(const vec3& v, const std::optional<int>& dp)
{
    std::stringstream ss;
    if (dp.has_value()) {
        ss << std::fixed << std::setprecision(dp.value());
    }
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return ss.str();
}

mat3 toMat3(const quat& q)
{
    return mat3(q);
}

quat toQuat(const mat3& m)
{
    return quat(m);
}

quat rotation(const vec3& axis, float degrees)
{
    return glm::rotate(glm::identity<quat>(), Maths::radians(degrees), axis);
}

quat rotation(const quat& orig, const vec3& axis, float degrees)
{
    return glm::rotate(orig, Maths::radians(degrees), axis);
}

}
}