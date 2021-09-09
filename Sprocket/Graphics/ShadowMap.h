#pragma once
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/frame_buffer.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace spkt {

class texture;

class ShadowMap
{
    asset_manager* d_assetManager;

    shader d_shader;

    glm::mat4 d_lightViewMatrix;
    glm::mat4 d_lightProjMatrix;

    frame_buffer d_shadowMap;

    spkt::vertex_buffer<spkt::model_instance> d_instance_buffer;

public:
    ShadowMap(asset_manager* assetManager);

    void Draw(spkt::registry& registry, const glm::vec3& sunDirection, const glm::vec3& centre);

    glm::mat4 GetLightProjViewMatrix() const;
    const texture&  GetShadowMap() const;
};

}