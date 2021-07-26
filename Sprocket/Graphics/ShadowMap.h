#pragma once
#include <Sprocket/Graphics/AssetManager.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/DepthBuffer.h>
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/VertexArray.h>
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace spkt {

class Texture;

class ShadowMap
{
    AssetManager* d_assetManager;

    shader d_shader;

    glm::mat4 d_lightViewMatrix;
    glm::mat4 d_lightProjMatrix;

    DepthBuffer d_shadowMap;

    spkt::buffer<model_instance> d_instance_buffer;

public:
    ShadowMap(AssetManager* assetManager);

    void Draw(spkt::registry& registry, const glm::vec3& sunDirection, const glm::vec3& centre);

    glm::mat4 GetLightProjViewMatrix() const;
    Texture*  GetShadowMap() const;
};

}