#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace spkt {

// Constructs a transformation matrix with the given parameters.
glm::mat4 make_transform(
    const glm::vec3& position,
    const glm::quat& orientation,
    const glm::vec3& scale = {1.0f, 1.0f, 1.0f}
);

glm::vec3 Forwards(const glm::quat& q);
glm::vec3 Right(const glm::quat& q);
glm::vec3 Up(const glm::quat& q);

void Decompose(const glm::mat4& matrix, glm::vec3* position, glm::quat* orientation, glm::vec3* scale);
glm::vec3 GetTranslation(const glm::mat4& m);
float Modulo(float val, float high);
glm::mat4 NoScale(const glm::mat4& matrix);
glm::vec3 ApplyTransform(const glm::mat4& matrix, const glm::vec3& v);

glm::vec3 get_mouse_ray(
    const glm::vec2& mouse_pos,
    float window_width,
    float window_height,
    const glm::mat4& view,
    const glm::mat4& proj
);

}