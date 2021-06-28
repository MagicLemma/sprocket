#include "Maths.h"
#include "Types.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/trigonometric.hpp>

namespace spkt {
namespace Maths {

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

glm::vec3 GetMouseRay(const glm::vec2& mousePos, float w, float h, const glm::mat4& view, const glm::mat4& proj)
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

}
}