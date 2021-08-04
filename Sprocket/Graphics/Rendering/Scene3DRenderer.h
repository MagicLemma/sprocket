#pragma once
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/ShadowMap.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Scene/ecs.h>

#include <memory>

namespace spkt {

class AssetManager;
class VertexArray;

class Scene3DRenderer
// Renders a scene as a 3D Scene. This makes use of components such as
// Transform3DComponent and ModelComponent, and will ignore 2D components
// such as Transform2DComponent and SpriteComponent.
{
public:
    // PBR Texture Slots
    static constexpr int ALBEDO_SLOT = 0;
    static constexpr int NORMAL_SLOT = 1;
    static constexpr int METALLIC_SLOT = 2;
    static constexpr int ROUGHNESS_SLOT = 3;

    // Shadow Map Texture Slot
    static constexpr int SHADOW_MAP_SLOT = 4;

    // Animation Data
    static constexpr int MAX_BONES = 50;

    // Light Data
    static constexpr int MAX_NUM_LIGHTS = 5;

private:
    AssetManager*    d_assetManager;

    shader d_staticShader;
    shader d_animatedShader;
    
    spkt::vertex_buffer<spkt::model_instance> d_instanceBuffer;

    Scene3DRenderer(const Scene3DRenderer&) = delete;
    Scene3DRenderer& operator=(const Scene3DRenderer&) = delete;

public:
    Scene3DRenderer(AssetManager* assetManager);

    void Draw(const spkt::registry& registry, spkt::entity camera);
    void Draw(const spkt::registry& registry, const glm::mat4& proj, const glm::mat4& view);

    void EnableShadows(const ShadowMap& shadowMap);

    shader& GetShader() { return d_staticShader; }
};

}