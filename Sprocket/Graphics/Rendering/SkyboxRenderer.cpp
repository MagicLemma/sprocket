#include "SkyboxRenderer.h"

#include <Sprocket/Graphics/camera.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/cube_map.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {
    
skybox_renderer::skybox_renderer(asset_manager* asset_manager)
    : d_asset_manager(asset_manager)
    , d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
{
}

void skybox_renderer::draw(const cube_map& skybox, const glm::mat4& proj, const glm::mat4& view)
{
    d_shader.bind();
    d_shader.load("projectionMatrix", proj);

    // Remove the translation so the camera never escapes the skybox.
    d_shader.load("viewMatrix", glm::mat4(glm::mat3(view)));

    skybox.bind();
    spkt::draw(d_asset_manager->get<static_mesh>("Resources/Models/Skybox.obj"));
}

}