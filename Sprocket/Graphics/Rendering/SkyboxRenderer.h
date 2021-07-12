#pragma once
#include "CubeMap.h"
#include "AssetManager.h"
#include "Shader.h"
#include "VertexArray.h"

#include "ecs.h"

#include <memory>

namespace spkt {

class SkyboxRenderer
{
    AssetManager* d_assetManager;

    Shader d_shader;
        // Shader used to draw entities.

    std::unique_ptr<VertexArray> d_vao;

public:
    SkyboxRenderer(AssetManager* assetManager);

    void Draw(const CubeMap& skybox, spkt::registry& registry, spkt::entity camera);
    void Draw(const CubeMap& skybox, const glm::mat4& proj, const glm::mat4& view);
};

}