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
glm::mat4 Translate(const glm::mat4& matrix, const glm::vec3& translation)
{
    return glm::translate(matrix, translation);
}

glm::mat4 Scale(const glm::mat4& matrix, const glm::vec3& scales)
{
    return glm::scale(matrix, scales);
}

glm::mat4 Scale(const glm::mat4& matrix, float scale)
{
    return glm::scale(matrix, {scale, scale, scale});
}

glm::mat4 Rotate(const glm::mat4& matrix, const glm::vec3& axis, float radians)
{
    return glm::rotate(matrix, radians, axis);
}

glm::mat4 Transpose(const glm::mat4& matrix)
{
    return glm::transpose(matrix);
}

// Matrix Constructors
glm::mat4 Transform(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
{
    glm::mat4 m = ToMat3(orientation);
    m[3][0] = position.x;
    m[3][1] = position.y;
    m[3][2] = position.z;
    m[3][3] = 1.0f;
    return Maths::Scale(m, scale);
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

glm::mat4 LookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
    return glm::lookAt(position, target, up);
}

glm::mat4 Ortho(float left, float right, float bottom, float top)
{
    return glm::ortho(left, right, bottom, top);   
}

glm::mat4 Ortho(float left, float right, float bottom, float top, float near, float far)
{
    return glm::ortho(left, right, bottom, top, near, far);   
}

glm::mat4 Ortho(float width, float height, float length)
{
    glm::mat4 m(0.0f);
    m[0][0] = 2.0f / width;
    m[1][1] = 2.0f / height;
    m[2][2] = -2.0f / length;
    m[3][3] = 1.0f;
    return m;
}

// Quaternion Modifiers
glm::quat Rotate(const glm::vec3& axis, float degrees)
{
    return glm::rotate(Maths::identity, glm::radians(degrees), axis);
}

glm::quat Rotate(const glm::quat& orig, const glm::vec3& axis, float degrees)
{
    return glm::rotate(orig, glm::radians(degrees), axis);
}

glm::quat Normalise(const glm::quat& q)
{
    return glm::normalize(q);
}

glm::quat LookAtQuat(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
    glm::mat4 lookAtMat = LookAt(position, target, up);
    glm::mat3 rotation = lookAtMat;
    return glm::conjugate(ToQuat(rotation));
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
glm::mat3 ToMat3(const glm::quat& q)
{
    return glm::mat3_cast(q);
}

glm::mat4 ToMat4(const glm::quat& q)
{
    return glm::mat4_cast(q);
}

glm::quat ToQuat(const glm::mat3& m)
{
    return glm::quat_cast(m);
}

glm::vec3 ToEuler(const glm::quat& q)
{
    return glm::eulerAngles(q);
}

void Decompose(const glm::mat4& matrix, glm::vec3* position, glm::quat* orientation, glm::vec3* scale)
{
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, *scale, *orientation, *position, skew, perspective);
}

float* Cast(const glm::mat3& m)
{
    return (float*)&m[0][0];
}

float* Cast(const glm::mat4& m)
{
    return (float*)&m[0][0];
}

// Vector Maths
glm::vec3 Cross(const glm::vec3& lhs, const glm::vec3& rhs)
{
    return glm::cross(lhs, rhs);
}

glm::vec3 GetTranslation(const glm::mat4& m)
{
    return m[3];
}

float Length(const glm::vec3& v)
{
    return glm::length(v);
}

float Length(const glm::vec2& v)
{
    return glm::length(v);
}

float Length(const glm::ivec2& v)
{
    return (float)glm::sqrt(v.x * v.x + v.y * v.y);
}


float LengthSquare(const glm::vec3& v)
{
    return glm::length2(v);
}

glm::vec3 Normalise(const glm::vec3& vec)
{
    return glm::normalize(vec);
}

glm::vec3 Interpolate(const glm::vec3& a, const glm::vec3& b, float delta)
{
    return glm::mix(a, b, delta);
}

glm::quat Interpolate(const glm::quat& a, const glm::quat& b, float delta)
{
    return glm::slerp(a, b, delta);
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
    return Normalise(glm::inverse(view) * ray);
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