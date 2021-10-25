#pragma once
#include <sprocket/graphics/shader.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {

class geometry_renderer
{
    spkt::shader d_shader;

public:
    geometry_renderer();

    void begin_frame(const glm::mat4& proj, const glm::mat4& view) const;
    void end_frame() const;

    void draw_box(const glm::mat4& transform, const glm::vec3& half_extents) const;
    void draw_sphere(const glm::mat4& transform, const float radius) const;
    void draw_capsule(const glm::mat4& transform, const float radius, const float height) const;
};

}