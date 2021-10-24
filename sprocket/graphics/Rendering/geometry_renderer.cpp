#include "geometry_renderer.h"

#include <sprocket/Graphics/camera.h>
#include <sprocket/Graphics/open_gl.h>
#include <sprocket/Graphics/render_context.h>
#include <sprocket/Utility/Maths.h>

#include <glad/glad.h>

namespace spkt {

geometry_renderer::geometry_renderer()
    : d_shader("Resources/Shaders/Collider.vert", "Resources/Shaders/Collider.frag")
{
}

void geometry_renderer::begin_frame(const glm::mat4& proj, const glm::mat4& view) const
{
    d_shader.bind();
    d_shader.load("u_proj_matrix", proj);
    d_shader.load("u_view_matrix", view);
}

void geometry_renderer::end_frame() const
{
    d_shader.unbind();
}

void geometry_renderer::draw_box(const glm::mat4& transform, const glm::vec3& half_extents) const
{
    static const static_mesh s_cube{"Resources/Models/Cube.obj"};
    d_shader.load("u_model_matrix", glm::scale(transform, half_extents));
    spkt::draw(s_cube);
}

void geometry_renderer::draw_sphere(const glm::mat4& transform, const float radius) const
{
    static const static_mesh s_sphere{"Resources/Models/LowPolySphere.obj"};
    d_shader.load("u_model_matrix", glm::scale(transform, {radius, radius, radius}));
    spkt::draw(s_sphere);
}

void geometry_renderer::draw_capsule(const glm::mat4& base_transform, const float radius, const float height) const
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