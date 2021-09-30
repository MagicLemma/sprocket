#include "ColliderRenderer.h"

#include <Sprocket/Graphics/camera.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Utility/Maths.h>

#include <glad/glad.h>

namespace spkt {

collider_renderer::collider_renderer()
    : d_shader("Resources/Shaders/Collider.vert", "Resources/Shaders/Collider.frag")
{
}

std::unique_ptr<spkt::render_context> collider_renderer::begin_frame(
    const glm::mat4& proj, const glm::mat4& view) const
{
    auto rc = std::make_unique<spkt::render_context>();
    rc->wireframe(true);

    d_shader.bind();
    d_shader.load("u_proj_matrix", proj);
    d_shader.load("u_view_matrix", view);

    return rc;
}

void collider_renderer::end_frame() const
{
    d_shader.unbind();
}

void collider_renderer::draw_box(const glm::mat4& transform, const glm::vec3& half_extents) const
{
    static const static_mesh s_cube{"Resources/Models/Cube.obj"};
    d_shader.load("u_model_matrix", glm::scale(transform, half_extents));
    spkt::draw(s_cube);
}

void collider_renderer::draw_sphere(const glm::mat4& transform, const float radius) const
{
    static const static_mesh s_sphere{"Resources/Models/LowPolySphere.obj"};
    d_shader.load("u_model_matrix", glm::scale(transform, {radius, radius, radius}));
    spkt::draw(s_sphere);
}

void collider_renderer::draw_capsule(const glm::mat4& base_transform, const float radius, const float height) const
{
    static const static_mesh s_hemisphere{"Resources/Models/Hemisphere.obj"};
    static const static_mesh s_cylinder{"Resources/Models/Cylinder.obj"};

    {  // Top Hemisphere
        auto transform = glm::translate(base_transform, {0.0, height/2, 0.0});
        transform = glm::scale(transform, {radius, radius, radius});
        d_shader.load("u_model_matrix", transform);
        spkt::draw(s_hemisphere);
    }

    {  // Middle Cylinder
        auto transform = glm::scale(base_transform, {radius, height, radius});
        d_shader.load("u_model_matrix", transform);
        spkt::draw(s_cylinder);
    }

    {  // Bottom Hemisphere
        auto transform = glm::translate(base_transform, {0.0, -height/2, 0.0});
        transform = glm::rotate(transform, glm::pi<float>(), {1, 0, 0});
        transform = glm::scale(transform, {radius, radius, radius});
        d_shader.load("u_model_matrix", transform);
        spkt::draw(s_hemisphere);
    }
}

}