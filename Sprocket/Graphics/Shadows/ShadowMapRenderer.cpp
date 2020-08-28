#include "ShadowMapRenderer.h"
#include "Components.h"

#include <glad/glad.h>

namespace Sprocket {

ShadowMapRenderer::ShadowMapRenderer(Window* window, ModelManager* modelManager)
    : d_window(window)
    , d_modelManager(modelManager)
    , d_shader("Resources/Shaders/ShadowMap.vert", "Resources/Shaders/ShadowMap.frag")
    , d_lightViewMatrix() // Will be populated after starting a scene.
    , d_lightProjMatrix(Maths::Ortho(-25.0f, 25.0f, -25.0f, 25.0f, -20.0f, 20.0f))
    , d_shadowMap(window, 4096, 4096)
{
}

void ShadowMapRenderer::BeginScene(const DirectionalLight& light,
                                   const Maths::vec3& centre)
{
    d_lightViewMatrix = Maths::LookAt(centre - light.direction, centre);

    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", d_lightProjMatrix);
    d_shader.LoadUniform("u_view_matrix", d_lightViewMatrix);

    d_shadowMap.Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapRenderer::Draw(const Entity& entity)
{
    if (!entity.Has<TransformComponent>()) { return; }
    if (!entity.Has<ModelComponent>()) { return; }
    const auto& modelComponent = entity.Get<ModelComponent>();

    auto tr = entity.Get<TransformComponent>();
    Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation, tr.scale);
    d_shader.LoadUniform("u_model_matrix", transform);

    auto model = d_modelManager->GetModel(modelComponent.model);

    model.Bind();
    glCullFace(GL_FRONT);
    glDrawElements(GL_TRIANGLES, (int)model.VertexCount(), GL_UNSIGNED_INT, nullptr);
    glCullFace(GL_BACK);
    model.Unbind();
}

void ShadowMapRenderer::EndScene()
{
    d_shadowMap.Unbind();
    d_shader.Unbind();
}

Maths::mat4 ShadowMapRenderer::GetLightProjViewMatrix() const
{
    return d_lightProjMatrix * d_lightViewMatrix;
}

Texture ShadowMapRenderer::GetShadowMap() const
{
    return d_shadowMap.GetShadowMap();
}

}