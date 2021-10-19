#pragma once
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/shader.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {

class cube_map;

class skybox_renderer
{
    asset_manager* d_asset_manager;
    shader         d_shader;

public:
    skybox_renderer(asset_manager* asset_manager);

    void draw(const cube_map& skybox, const glm::mat4& proj, const glm::mat4& view);
};

}