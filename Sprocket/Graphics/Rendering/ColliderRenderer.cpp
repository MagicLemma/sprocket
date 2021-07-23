#include "ColliderRenderer.h"

#include <Sprocket/Graphics/RenderContext.h>
#include <Sprocket/Graphics/VertexArray.h>
#include <Sprocket/Scene/Camera.h>
#include <Sprocket/Utility/Maths.h>

#include <glad/glad.h>

namespace spkt {

ColliderRenderer::ColliderRenderer()
    : d_vao(std::make_unique<VertexArray>())
    , d_shader("Resources/Shaders/Collider.vert", "Resources/Shaders/Collider.frag")
{
}

void ColliderRenderer::Draw(
    const spkt::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view)
{
    RenderContext rc;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::uint32_t MAX_NUM_LIGHTS = 5;

    d_shader.Bind();
    d_shader.load("u_proj_matrix", proj);
    d_shader.load("u_view_matrix", view);
    
    static auto s_cube = Mesh::FromFile("Resources/Models/Cube.obj");
    d_vao->SetModel(s_cube.get());
    for (auto entity : registry.view<BoxCollider3DComponent>()) {
        const auto& c = registry.get<BoxCollider3DComponent>(entity);
        auto tr = registry.get<Transform3DComponent>(entity);
        glm::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(c.position, c.orientation);
        transform = glm::scale(transform, c.halfExtents);
        if (c.applyScale) {
            transform = glm::scale(transform, tr.scale);
        }
        d_shader.load("u_model_matrix", transform);
        d_vao->Draw();
    }

    static auto s_sphere = Mesh::FromFile("Resources/Models/LowPolySphere.obj");
    d_vao->SetModel(s_sphere.get());
    for (auto entity : registry.view<SphereCollider3DComponent>()) {
        const auto& c = registry.get<SphereCollider3DComponent>(entity);
        auto tr = registry.get<Transform3DComponent>(entity);
        glm::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(c.position, c.orientation);
        transform = glm::scale(transform, {c.radius, c.radius, c.radius});
        d_shader.load("u_model_matrix", transform);
        d_vao->Draw();
    }

    static auto s_hemisphere = Mesh::FromFile("Resources/Models/Hemisphere.obj");
    static auto s_cylinder = Mesh::FromFile("Resources/Models/Cylinder.obj");

    for (auto entity : registry.view<CapsuleCollider3DComponent>()) {
        const auto& c = registry.get<CapsuleCollider3DComponent>(entity);

        {  // Top Hemisphere
            auto tr = registry.get<Transform3DComponent>(entity);
            glm::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = glm::translate(transform, {0.0, c.height/2, 0.0});
            transform = glm::scale(transform, {c.radius, c.radius, c.radius});
            d_shader.load("u_model_matrix", transform);
            d_vao->SetModel(s_hemisphere.get());
            d_vao->Draw();
        }

        {  // Middle Cylinder
            auto tr = registry.get<Transform3DComponent>(entity);
            glm::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = glm::scale(transform, {c.radius, c.height, c.radius});
            d_shader.load("u_model_matrix", transform);
            d_vao->SetModel(s_cylinder.get());
            d_vao->Draw();
        }

        {  // Bottom Hemisphere
            auto tr = registry.get<Transform3DComponent>(entity);
            glm::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = glm::translate(transform, {0.0, -c.height/2, 0.0});
            transform = glm::rotate(transform, glm::pi<float>(), {1, 0, 0});
            transform = glm::scale(transform, {c.radius, c.radius, c.radius});
            d_shader.load("u_model_matrix", transform);
            d_vao->SetModel(s_hemisphere.get());
            d_vao->Draw();
        }
    }

    d_shader.Unbind();
}

void ColliderRenderer::Draw(const spkt::registry& registry, spkt::entity camera)
{
    glm::mat4 proj = spkt::make_proj(registry, camera);
    glm::mat4 view = spkt::make_view(registry, camera);
    Draw(registry, proj, view);
}

}