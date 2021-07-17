#pragma once
#include <Sprocket/Graphics/Shader.h>
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {

class AssetManager;
class CubeMap;
class VertexArray;

class SkyboxRenderer
{
    AssetManager* d_assetManager;

    Shader d_shader;
        // Shader used to draw entities.

    std::unique_ptr<VertexArray> d_vao;

public:
    SkyboxRenderer(AssetManager* assetManager);

    void Draw(const CubeMap& skybox, const spkt::registry& registry, spkt::entity camera);
    void Draw(const CubeMap& skybox, const glm::mat4& proj, const glm::mat4& view);
};

}