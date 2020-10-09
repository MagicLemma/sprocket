#pragma once
#include "Entity.h"
#include "Shader.h"
#include "Texture.h"
#include "AssetManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "Components.h"
#include "VertexArray.h"
#include "ShadowMap.h"
#include "ParticleManager.h"

namespace Sprocket {

class EntityRenderer
{
    AssetManager*    d_assetManager;
    MaterialManager* d_materialManager;
    TextureManager*  d_textureManager;
    ParticleManager* d_particleManager;

    Shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;
    
    std::shared_ptr<Buffer> d_instanceBuffer;
    std::vector<InstanceData> d_instanceData;

public:
    EntityRenderer(
        AssetManager* assetManager,
        TextureManager* textureManager,
        MaterialManager* materialManager
    );

    void Draw(const Entity& camera, Scene& scene);
    void Draw(const Maths::mat4& proj, const Maths::mat4& view, Scene& scene);

    void EnableShadows(const ShadowMap& shadowMap);
    void EnableParticles(ParticleManager* particleManager);

    Shader& GetShader() { return d_shader; }
};

}