#include "SkyboxRenderer.h"

#include <Sprocket/Graphics/AssetManager.h>
#include <Sprocket/Graphics/CubeMap.h>
#include <Sprocket/Graphics/VertexArray.h>
#include <Sprocket/Scene/Camera.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {
    
SkyboxRenderer::SkyboxRenderer(AssetManager* assetManager)
    : d_assetManager(assetManager)
    , d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
    , d_vao(std::make_unique<VertexArray>())
{
}

void SkyboxRenderer::Draw(const CubeMap& skybox,
                          const glm::mat4& proj,
                          const glm::mat4& view)
{
    d_shader.Bind();
    d_shader.LoadMat4("projectionMatrix", proj);

    // Remove the translation so the camera never escapes the skybox.
    d_shader.LoadMat4("viewMatrix", glm::mat4(glm::mat3(view)));

    skybox.Bind();
    d_vao->SetModel(d_assetManager->GetMesh("Resources/Models/Skybox.obj"));
    d_vao->Draw();
}

void SkyboxRenderer::Draw(const CubeMap& skybox, const spkt::registry& registry, spkt::entity camera)
{
    Draw(
        skybox,
        spkt::make_proj(registry, camera),
        spkt::make_view(registry, camera)
    );
}

}