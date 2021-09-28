#include "SkyboxRenderer.h"

#include <Sprocket/Graphics/camera.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/CubeMap.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {
    
SkyboxRenderer::SkyboxRenderer(asset_manager* assetManager)
    : d_assetManager(assetManager)
    , d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
{
}

void SkyboxRenderer::Draw(const CubeMap& skybox,
                          const glm::mat4& proj,
                          const glm::mat4& view)
{
    d_shader.bind();
    d_shader.load("projectionMatrix", proj);

    // Remove the translation so the camera never escapes the skybox.
    d_shader.load("viewMatrix", glm::mat4(glm::mat3(view)));

    skybox.Bind();
    spkt::draw(d_assetManager->get<static_mesh>("Resources/Models/Skybox.obj"));
}

void SkyboxRenderer::Draw(const CubeMap& skybox, const spkt::registry& registry, spkt::entity camera)
{
    auto [tc, cc] = registry.get_all<spkt::Transform3DComponent, spkt::Camera3DComponent>(camera);
    glm::mat4 view = spkt::make_view(tc.position, tc.orientation, cc.pitch);
    glm::mat4 proj = spkt::make_proj(cc.fov);
    Draw(skybox, proj, view);
}

}