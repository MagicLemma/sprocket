#include "SkyboxRenderer.h"
#include "Camera.h"

namespace Sprocket {
    
SkyboxRenderer::SkyboxRenderer(AssetManager* assetManager)
    : d_assetManager(assetManager)
    , d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
    , d_vao(std::make_unique<VertexArray>())
{
}

void SkyboxRenderer::Draw(const CubeMap& skybox,
                          const Maths::mat4& proj,
                          const Maths::mat4& view)
{
    d_shader.Bind();
    d_shader.LoadMat4("projectionMatrix", proj);
    d_shader.LoadMat4("viewMatrix", view);

    skybox.Bind();
    d_vao->SetModel(d_assetManager->GetMesh("Resources/Models/Skybox.obj"));
    d_vao->Draw();
}

void SkyboxRenderer::Draw(const CubeMap& skybox, const Entity& camera)
{
    Maths::mat4 view = MakeView(camera);
    Maths::mat4 proj = MakeProj(camera);
    Maths::mat4 view2 = Maths::mat4(Maths::mat3(view));
    Draw(skybox, proj, view);
}

}