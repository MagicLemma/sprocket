#include "ColliderRenderer.h"

#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Scene/camera.h>
#include <Sprocket/Utility/Maths.h>

#include <glad/glad.h>

namespace spkt {

ColliderRenderer::ColliderRenderer()
    : d_shader("Resources/Shaders/Collider.vert", "Resources/Shaders/Collider.frag")
{
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
    
    static const static_mesh s_cube{"Resources/Models/Cube.obj"};
    static const static_mesh s_sphere{"Resources/Models/LowPolySphere.obj"};
    static const static_mesh s_hemisphere{"Resources/Models/Hemisphere.obj"};
    static const static_mesh s_cylinder{"Resources/Models/Cylinder.obj"};

    for (auto [bc, tc] : registry.view_get<BoxCollider3DComponent, Transform3DComponent>()) {
        glm::mat4 transform = Maths::Transform(tc.position, tc.orientation);
        transform *= Maths::Transform(bc.position, bc.orientation);
        transform = glm::scale(transform, bc.halfExtents);
        if (bc.applyScale) {
            transform = glm::scale(transform, tc.scale);
        }
        d_shader.load("u_model_matrix", transform);
        spkt::draw(s_cube);
    }

    for (auto [sc, tr] : registry.view_get<SphereCollider3DComponent, Transform3DComponent>()) {
        glm::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(sc.position, sc.orientation);
        transform = glm::scale(transform, {sc.radius, sc.radius, sc.radius});
        d_shader.load("u_model_matrix", transform);
        spkt::draw(s_sphere);
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