#include "SkyboxRenderer.h"
#include "CameraUtils.h"

namespace Sprocket {
    
SkyboxRenderer::SkyboxRenderer(ModelManager* modelManager)
    : d_modelManager(modelManager)
    , d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
    , d_vao(std::make_unique<VertexArray>())
{
}

void SkyboxRenderer::Draw(const Skybox& skybox,
                          const Maths::mat4& proj,
                          const Maths::mat4& view)
{
    d_shader.Bind();
    d_shader.LoadMat4("projectionMatrix", proj);
    d_shader.LoadMat4("viewMatrix", view);

    skybox.texture.Bind();
    d_vao->SetModel(d_modelManager->GetModel("Resources/Models/Skybox.obj"));
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