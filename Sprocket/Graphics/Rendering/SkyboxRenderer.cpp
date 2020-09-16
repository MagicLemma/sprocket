#include "SkyboxRenderer.h"
#include "CameraUtils.h"

namespace Sprocket {
    
SkyboxRenderer::SkyboxRenderer()
    : d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
    , d_vao(std::make_unique<VertexArray>())
{
}

void SkyboxRenderer::Draw(const Skybox& skybox,
                          const Maths::mat4& proj,
                          const Maths::mat4& view)
{
    d_shader.Bind();
    d_shader.LoadUniformMat4("projectionMatrix", proj);
    d_shader.LoadUniformMat4("viewMatrix", view);

    skybox.texture.Bind();
    d_vao->SetModel(skybox.model);
    d_vao->Draw();
}

void SkyboxRenderer::Draw(const Skybox& skybox, const Entity& camera)
{
    Maths::mat4 view = CameraUtils::MakeView(camera);
    Maths::mat4 proj = CameraUtils::MakeProj(camera);
    Maths::mat4 view2 = Maths::mat4(Maths::mat3(view));
    Draw(skybox, proj, view);
}

}