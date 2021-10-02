#pragma once
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/shader.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {

class CubeMap;
class VertexArray;

class SkyboxRenderer
{
    asset_manager* d_assetManager;

    shader d_shader;

public:
    SkyboxRenderer(asset_manager* assetManager);

    void Draw(const CubeMap& skybox, const glm::mat4& proj, const glm::mat4& view);
};

}