#pragma once
#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Graphics/shader.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {

// TODO: Make this an instanced renderer, no need to return a render context pointer then.
class collider_renderer
{
    spkt::shader d_shader;

public:
    collider_renderer();

    std::unique_ptr<spkt::render_context> begin_frame(const glm::mat4& proj, const glm::mat4& view) const;
    void end_frame() const;

    void draw_box(const glm::mat4& transform, const glm::vec3& half_extents) const;
    void draw_sphere(const glm::mat4& transform, const float radius) const;
    void draw_capsule(const glm::mat4& transform, const float radius, const float height) const;
};

}