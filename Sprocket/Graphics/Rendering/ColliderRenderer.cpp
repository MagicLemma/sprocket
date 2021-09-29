#include "ColliderRenderer.h"

#include <Sprocket/Graphics/camera.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Utility/Maths.h>

#include <glad/glad.h>

namespace spkt {

ColliderRenderer::ColliderRenderer()
    : d_shader("Resources/Shaders/Collider.vert", "Resources/Shaders/Collider.frag")
{
}

void ColliderRenderer::draw_box(const glm::mat4& transform, const glm::vec3& half_extents)
{
    static const static_mesh s_cube{"Resources/Models/Cube.obj"};
    d_shader.load("u_model_matrix", glm::scale(transform, half_extents));
    spkt::draw(s_cube);
}

void ColliderRenderer::draw_sphere(const glm::mat4& transform, const float radius)
{
    static const static_mesh s_sphere{"Resources/Models/LowPolySphere.obj"};
    d_shader.load("u_model_matrix", glm::scale(transform, {radius, radius, radius}));
    spkt::draw(s_sphere);
}

void ColliderRenderer::Draw(
    const spkt::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view)
{
    spkt::render_context rc;
    rc.polygon_mode(GL_LINE);

    std::uint32_t MAX_NUM_LIGHTS = 5;

    d_shader.bind();
    d_shader.load("u_proj_matrix", proj);
    d_shader.load("u_view_matrix", view);
    
    
    static const static_mesh s_hemisphere{"Resources/Models/Hemisphere.obj"};
    static const static_mesh s_cylinder{"Resources/Models/Cylinder.obj"};

    for (auto [bc, tc] : registry.view_get<BoxCollider3DComponent, Transform3DComponent>()) {
        const glm::mat4 transform = Maths::Transform(tc.position, tc.orientation)
                                  * Maths::Transform(bc.position, bc.orientation);
        const glm::vec3 scale = bc.applyScale ? bc.halfExtents * tc.scale : bc.halfExtents;      
        draw_box(transform, scale);
    }

    for (auto [sc, tc] : registry.view_get<SphereCollider3DComponent, Transform3DComponent>()) {
        const glm::mat4 transform = Maths::Transform(tc.position, tc.orientation)
                                  * Maths::Transform(sc.position, sc.orientation);
        draw_sphere(transform, sc.radius);
    }

    for (auto [cc, tc] : registry.view_get<CapsuleCollider3DComponent, Transform3DComponent>()) {

        {  // Top Hemisphere
            glm::mat4 transform = Maths::Transform(tc.position, tc.orientation);
            transform *= Maths::Transform(cc.position, cc.orientation);
            transform = glm::translate(transform, {0.0, cc.height/2, 0.0});
            transform = glm::scale(transform, {cc.radius, cc.radius, cc.radius});
            d_shader.load("u_model_matrix", transform);
            spkt::draw(s_hemisphere);
        }

        {  // Middle Cylinder
            glm::mat4 transform = Maths::Transform(tc.position, tc.orientation);
            transform *= Maths::Transform(cc.position, cc.orientation);
            transform = glm::scale(transform, {cc.radius, cc.height, cc.radius});
            d_shader.load("u_model_matrix", transform);
            spkt::draw(s_cylinder);
        }

        {  // Bottom Hemisphere
            glm::mat4 transform = Maths::Transform(tc.position, tc.orientation);
            transform *= Maths::Transform(cc.position, cc.orientation);
            transform = glm::translate(transform, {0.0, -cc.height/2, 0.0});
            transform = glm::rotate(transform, glm::pi<float>(), {1, 0, 0});
            transform = glm::scale(transform, {cc.radius, cc.radius, cc.radius});
            d_shader.load("u_model_matrix", transform);
            spkt::draw(s_hemisphere);
        }
    }

    d_shader.unbind();
}

void ColliderRenderer::Draw(const spkt::registry& registry, spkt::entity camera)
{
    auto [tc, cc] = registry.get_all<spkt::Transform3DComponent, spkt::Camera3DComponent>(camera);
    glm::mat4 view = spkt::make_view(tc.position, tc.orientation, cc.pitch);
    glm::mat4 proj = spkt::make_proj(cc.fov);
    Draw(registry, proj, view);
}

}