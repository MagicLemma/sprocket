#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <tuple>

namespace spkt {

// Constructs a transformation matrix with the given parameters.
glm::mat4 make_transform(
    const glm::vec3& position,
    const glm::quat& orientation,
    const glm::vec3& scale = {1.0f, 1.0f, 1.0f}
);

glm::vec3 forwards(const glm::quat& q);
glm::vec3 right(const glm::quat& q);
glm::vec3 up(const glm::quat& q);

struct decomposed_transform
{
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;

    std::tuple<glm::vec3, glm::quat, glm::vec3> as_tuple() const
    {
        return {position, orientation, scale};
    }
};

decomposed_transform decompose(const glm::mat4 matrix);

glm::vec3 get_translation(const glm::mat4& m);
float modulo(float val, float high);
glm::mat4 no_scale(const glm::mat4& matrix);
glm::vec3 apply_transform(const glm::mat4& matrix, const glm::vec3& v);

glm::vec3 get_mouse_ray(
    const glm::vec2& mouse_pos,
    float window_width,
    float window_height,
    const glm::mat4& view,
    const glm::mat4& proj
);

}