#include "SkyboxRenderer.h"
#include "RenderContext.h"
#include "CameraUtils.h"

#include <glad/glad.h>

namespace Sprocket {
    
SkyboxRenderer::SkyboxRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
    , d_vao(std::make_unique<VertexArray>())
{
}

void SkyboxRenderer::Draw(const Skybox& skybox,
                          const Maths::mat4& proj,
                          const Maths::mat4& view)
{
    RenderContext rc;
    glDisable(GL_CULL_FACE);
    glDepthMask(true);

    d_shader.Bind();

    Maths::mat4 view2 = Maths::mat4(Maths::mat3(view));
    
    d_shader.LoadUniform("projectionMatrix", proj);
    d_shader.LoadUniform("viewMatrix", view2);

    //skybox.model->Bind();
    skybox.texture.Bind();

    d_vao->SetModel(skybox.model);
    d_vao->Draw();

    //glDrawElements(GL_TRIANGLES, skybox.model->VertexCount(), GL_UNSIGNED_INT, (const void*)0);
    skybox.texture.Unbind();
    //skybox.model->Unbind();

    d_shader.Unbind();
}

void SkyboxRenderer::Draw(const Skybox& skybox,
                          const Entity& camera)
{
    Maths::mat4 view = CameraUtils::MakeView(camera);
    Maths::mat4 proj = CameraUtils::MakeProj(camera);
    Draw(skybox, proj, view);
}

}