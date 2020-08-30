#include "ColliderRenderer.h"
#include "Maths.h"
#include "RenderContext.h"
#include "CameraUtils.h"
#include "Components.h"
#include "Texture.h"
#include "ModelManager.h"

#include <glad/glad.h>

namespace Sprocket {

ColliderRenderer::ColliderRenderer()
    : d_vao(std::make_unique<VertexArray>())
    , d_shader("Resources/Shaders/Collider.vert", "Resources/Shaders/Collider.frag")
{
}

void ColliderRenderer::Draw(
    const Maths::mat4& proj,
    const Maths::mat4& view,
    Scene& scene)
{
    RenderContext rc;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);
    d_shader.LoadUniform("u_view_matrix", view);
    
    static auto s_cube = ModelManager::LoadModel("Resources/Models/Cube.obj");
    d_vao->SetModel(s_cube);
    scene.Each<BoxCollider3DComponent>([&](Entity& entity) {
        const auto& c = entity.Get<BoxCollider3DComponent>();
        auto tr = entity.Get<TransformComponent>();
        Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(c.position, c.orientation);
        transform = Maths::Scale(transform, c.halfExtents);
        if (c.applyScale) {
            transform = Maths::Scale(transform, tr.scale);
        }
        d_shader.LoadUniform("u_model_matrix", transform);
        d_vao->Draw();
    });

    static auto s_sphere = ModelManager::LoadModel("Resources/Models/LowPolySphere.obj");
    d_vao->SetModel(s_sphere);
    scene.Each<SphereCollider3DComponent>([&](Entity& entity) {
        const auto& c = entity.Get<SphereCollider3DComponent>();
        auto tr = entity.Get<TransformComponent>();
        Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(c.position, c.orientation);
        transform = Maths::Scale(transform, c.radius);
        d_shader.LoadUniform("u_model_matrix", transform);
        d_vao->Draw();
    });

    static auto s_hemisphere = ModelManager::LoadModel("Resources/Models/Hemisphere.obj");
    static auto s_cylinder = ModelManager::LoadModel("Resources/Models/Cylinder.obj");
    d_vao->SetModel(s_sphere);
    scene.Each<CapsuleCollider3DComponent>([&](Entity& entity) {
        const auto& c = entity.Get<CapsuleCollider3DComponent>();

        {  // Top Hemisphere
            auto tr = entity.Get<TransformComponent>();
            Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = Maths::Translate(transform, {0.0, c.height/2, 0.0});
            transform = Maths::Scale(transform, c.radius);
            d_shader.LoadUniform("u_model_matrix", transform);
            d_vao->SetModel(s_hemisphere);
            d_vao->Draw();
        }

        {  // Middle Cylinder
            auto tr = entity.Get<TransformComponent>();
            Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = Maths::Scale(transform, {c.radius, c.height, c.radius});
            d_shader.LoadUniform("u_model_matrix", transform);
            d_vao->SetModel(s_cylinder);
            d_vao->Draw();
        }

        {  // Bottom Hemisphere
            auto tr = entity.Get<TransformComponent>();
            Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = Maths::Translate(transform, {0.0, -c.height/2, 0.0});
            transform = Maths::Rotate(transform, {1, 0, 0}, Maths::Radians(180.0f));
            transform = Maths::Scale(transform, c.radius);
            d_shader.LoadUniform("u_model_matrix", transform);
            d_vao->SetModel(s_hemisphere);
            d_vao->Draw();
        }
    });

    d_shader.Unbind();
}

void ColliderRenderer::Draw(const Entity& camera, Scene& scene)
{
    Maths::mat4 proj = CameraUtils::MakeProj(camera);
    Maths::mat4 view = CameraUtils::MakeView(camera);
    Draw(proj, view, scene);
}

}