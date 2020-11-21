#include "Maths.h"
#include "Types.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/trigonometric.hpp>

namespace Sprocket {
namespace Maths {

// Matrix Modifiers
glm::mat4 Rotate(const glm::mat4& matrix, const glm::vec3& axis, float radians)
{
    return glm::rotate(matrix, radians, axis);
}

// Matrix Constructors
glm::mat4 Transform(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
{
    glm::mat4 m = glm::mat4_cast(orientation);
    m[3][0] = position.x;
    m[3][1] = position.y;
    m[3][2] = position.z;
    m[3][3] = 1.0f;
    return glm::scale(m, scale);
}

glm::mat4 Perspective(float aspectRatio, float fov, float nearPlane, float farPlane)
{
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);  
}

glm::mat4 View(const glm::vec3& position, float pitch, float yaw, float roll)
{
    glm::mat4 matrix(1.0);
    matrix = glm::rotate(matrix, glm::radians(pitch), glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, glm::radians(yaw), glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, glm::radians(roll), glm::vec3(0, 0, 1));
    matrix = glm::translate(matrix, -position);
    return matrix;
}

// Quaternion Modifiers
glm::quat Rotate(const glm::vec3& axis, float degrees)
{
    return glm::rotate(glm::identity<glm::quat>(), glm::radians(degrees), axis);
}

glm::quat Rotate(const glm::quat& orig, const glm::vec3& axis, float degrees)
{
    return glm::rotate(orig, glm::radians(degrees), axis);
}

glm::vec3 Forwards(const glm::quat& q)
{
    return glm::normalize(q) * glm::vec3(0, 0, -1);
}

glm::vec3 Right(const glm::quat& q)
{
    return glm::normalize(q) * glm::vec3(1, 0, 0);
}

glm::vec3 Up(const glm::quat& q)
{
    return glm::normalize(q) * glm::vec3(0, 1, 0);
}

// Conversions
void Decompose(const glm::mat4& matrix, glm::vec3* position, glm::quat* orientation, glm::vec3* scale)
{
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, *scale, *orientation, *position, skew, perspective);
}

// Vector Maths
glm::vec3 GetTranslation(const glm::mat4& m)
{
    return m[3];
}

// Trig
float Sind(float degrees)
{
    return glm::sin(glm::radians(degrees));
}

float Cosd(float degrees)
{
    return glm::cos(glm::radians(degrees));
}

// General Helpers
float Modulo(float val, float high)
{
    float ret = val;
    while (ret > high) { ret -= high; }
    while (ret < 0) { ret += high; }
    return ret;
}

glm::mat4 NoScale(const glm::mat4& matrix)
{
    glm::vec3 pos, scale;
    glm::quat ori;
    Maths::Decompose(matrix, &pos, &ori, &scale);
    return Maths::Transform(pos, ori);
}

glm::vec3 GetMouseRay(const glm::vec2& mousePos, u32 w, u32 h, const glm::mat4& view, const glm::mat4& proj)
{
    // Homogeneous Clip Space
    glm::vec4 ray = {(2.0f * mousePos.x) / w - 1, -((2.0f * mousePos.y) / h - 1), -1.0f, 1.0f};

    // Eye Space
    ray = glm::inverse(proj) * ray;
    ray.z = -1.0f;
    ray.w = 0.0f;

    // World Space
    return glm::normalize(glm::inverse(view) * ray);
}

glm::vec3 ApplyTransform(const glm::mat4& matrix, const glm::vec3& v)
{
    glm::vec4 v2 = matrix * glm::vec4{v.x, v.y, v.z, 1.0f};
    return {v2.x, v2.y, v2.z};
}

// Printing
std::string ToString(const glm::vec3& v, const std::optional<int>& dp)
{
    std::stringstream ss;
    if (dp.has_value()) {
        ss << std::fixed << std::setprecision(dp.value());
    }
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return ss.str();
}

std::string ToString(float x, const std::optional<int>& dp)
{
    std::stringstream ss;
    if (dp.has_value()) {
        ss << std::fixed << std::setprecision(dp.value());
    }
    ss << x;
    return ss.str();
}

std::string ToString(bool t)
{
    if (t) {
        return "True";
    }
    return "False";
}

}
}