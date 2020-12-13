#pragma once
#include "CubeMap.h"
#include "AssetManager.h"
#include "ECS.h"
#include "Shader.h"
#include "VertexArray.h"

#include <memory>

namespace Sprocket {

class SkyboxRenderer
{
    AssetManager* d_assetManager;

    Shader d_shader;
        // Shader used to draw entities.

    std::unique_ptr<VertexArray> d_vao;

public:
    SkyboxRenderer(AssetManager* assetManager);

    void Draw(const CubeMap& skybox, const ECS::Entity& camera);
    void Draw(const CubeMap& skybox, const glm::mat4& proj, const glm::mat4& view);
};

}