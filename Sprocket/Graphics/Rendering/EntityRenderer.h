#pragma once
#include "Entity.h"
#include "Light.h"
#include "Shader.h"
#include "Texture.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "Components.h"
#include "VertexArray.h"
#include "ShadowMap.h"
#include "ParticleManager.h"

namespace Sprocket {

class EntityRenderer
{
    ModelManager*    d_modelManager;
    MaterialManager* d_materialManager;
    ParticleManager* d_particleManager;

    Shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;
    
    std::shared_ptr<Buffer> d_instanceBuffer;
    std::vector<InstanceData> d_instanceData;

public:
    EntityRenderer(
        ModelManager* modelManager,
        MaterialManager* materialManager
    );

    void Draw(const Entity& camera, const Lights& lights, Scene& scene);
    void Draw(const Maths::mat4& proj, const Maths::mat4& view, const Lights& lights, Scene& scene);

    void EnableShadows(const ShadowMap& shadowMap);
    void EnableParticles(ParticleManager* particleManager);

    Shader& GetShader() { return d_shader; }
};

}