#include "ShadowMapRenderer.h"

#include <glad/glad.h>

namespace Sprocket {

ShadowMapRenderer::ShadowMapRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/ShadowMap.vert",
               "Resources/Shaders/ShadowMap.frag")
{
}

void ShadowMapRenderer::OnUpdate(const DirectionalLight& light)
{
    static const float pi = 2.0f * (float)std::asin(1);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    Maths::mat4 lightView = Maths::LookAt(-light.direction, {0.0, 0.0, 0.0});
    Maths::mat4 lightProj = Maths::Ortho(-10.0f, 10.0f, -10.0f, 10.0f, -150.0f, 150.0f);

    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", lightProj);
    d_shader.LoadUniform("u_view_matrix", lightView);
    d_shader.Unbind();
}

void ShadowMapRenderer::Draw(const Entity& entity)
{
    if (!entity.Has<ModelComponent>()) { return; }
    const auto& model = entity.Get<ModelComponent>().model;

    d_shader.Bind();

    Maths::mat4 transform = entity.Transform();
    transform = Maths::Scale(transform, entity.Get<ModelComponent>().scale);
    d_shader.LoadUniform("u_model_matrix", transform);

    model.Bind();
    glCullFace(GL_FRONT);
    glDrawElements(GL_TRIANGLES, (int)model.VertexCount(), GL_UNSIGNED_INT, nullptr);
    glCullFace(GL_BACK);
    model.Unbind();
    d_shader.Unbind();
}

}