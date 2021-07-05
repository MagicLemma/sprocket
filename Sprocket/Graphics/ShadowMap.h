#pragma once
#include "Shader.h"
#include "DepthBuffer.h"
#include "AssetManager.h"
#include "Scene.h"
#include "VertexArray.h"

#include "apecs.hpp"

#include <memory>

namespace spkt {

class ShadowMap
{
    AssetManager*   d_assetManager;

    Shader  d_shader;

    glm::mat4 d_lightViewMatrix;
    glm::mat4 d_lightProjMatrix;

    DepthBuffer d_shadowMap;

    std::unique_ptr<VertexArray> d_vao;
    std::unique_ptr<Buffer> d_instanceBuffer;

    std::vector<InstanceData> d_instanceData;

public:
    ShadowMap(AssetManager* assetManager);

    void Draw(apx::registry& registry, const glm::vec3& sunDirection, const glm::vec3& centre);

    glm::mat4 GetLightProjViewMatrix() const;
    Texture*  GetShadowMap() const;
};

}