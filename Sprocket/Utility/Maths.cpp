#include "Maths.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/trigonometric.hpp>

namespace spkt {

glm::mat4 make_transform(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale)
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

decomposed_transform decompose(const glm::mat4 matrix)
{
    decomposed_transform dt;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, dt.scale, dt.orientation, dt.position, skew, perspective);
    return dt;
}

glm::vec3 GetTranslation(const glm::mat4& m)
{
    return m[3];
}

float Modulo(float val, float high)
{
    float ret = val;
    while (ret > high) { ret -= high; }
    while (ret < 0) { ret += high; }
    return ret;
}

glm::mat4 NoScale(const glm::mat4& matrix)
{
    auto [pos, ori, scale] = decompose(matrix);
    return make_transform(pos, ori);
}

glm::vec3 get_mouse_ray(
    const glm::vec2& mouse_position,
    float window_width,
    float window_height,
    const glm::mat4& view,
    const glm::mat4& proj)
{
    // Homogeneous Clip Space
    glm::vec4 ray = {
        (2.0f * mouse_position.x) / window_width - 1,
        -((2.0f * mouse_position.y) / window_height - 1),
        -1.0f,
        1.0f
    };

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