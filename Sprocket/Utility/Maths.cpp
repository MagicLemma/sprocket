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

float radians(float degrees)
{
    return glm::radians(degrees);
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

}
}