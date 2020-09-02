#include "EntityRenderer.h"
#include "Maths.h"
#include "ModelManager.h"
#include "RenderContext.h"
#include "CameraUtils.h"
#include "Components.h"
#include "Scene.h"
#include "ShadowMap.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

std::shared_ptr<InstanceBuffer> GetInstanceBuffer()
{
    BufferLayout layout(sizeof(InstanceData), 3);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 4, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 1, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 1, DataRate::INSTANCE);
    assert(layout.Validate());

    return std::make_shared<InstanceBuffer>(layout);
}

}

EntityRenderer::EntityRenderer(ModelManager* modelManager,
                               TextureManager* textureManager)
    : d_vao(std::make_unique<VertexArray>())
    , d_modelManager(modelManager)
    , d_textureManager(textureManager)
    , d_particleManager(nullptr)
    , d_shader("Resources/Shaders/Entity.vert", "Resources/Shaders/Entity.frag")
    , d_instanceBuffer(GetInstanceBuffer())
{
}

void EntityRenderer::EnableShadows(const ShadowMap& shadowMap)
{
    glActiveTexture(GL_TEXTURE3);
    shadowMap.GetShadowMap().Bind();
 
    d_shader.Bind();
    d_shader.LoadUniformInt("shadow_map", 3);
    d_shader.LoadUniform("u_light_proj_view", shadowMap.GetLightProjViewMatrix());
    glActiveTexture(GL_TEXTURE0);
}

void EntityRenderer::Draw(
    const Maths::mat4& proj,
    const Maths::mat4& view,
    const Lights& lights,
    Scene& scene)
{
    RenderContext rc;
    rc.FaceCulling(true);
    rc.DepthTesting(true);

    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);
    d_shader.LoadUniform("u_view_matrix", view);

    // Load sun to shader
    const auto& sun = scene.GetSun();
    d_shader.LoadUniform("u_sun_direction", sun.direction);
    d_shader.LoadUniform("u_sun_colour", sun.colour);
    d_shader.LoadUniform("u_sun_brightness", sun.brightness);

    // Load ambience to shader
    d_shader.LoadUniform("u_ambience_colour", lights.ambience.colour);
    d_shader.LoadUniform("u_ambience_brightness", lights.ambience.brightness);
    
    // Load point lights to shader
    std::size_t i = 0;
    scene.Each<TransformComponent, LightComponent>([&](Entity& entity) {
        if (i < MAX_NUM_LIGHTS) {
            auto position = entity.Get<TransformComponent>().position;
            auto light = entity.Get<LightComponent>();
            d_shader.LoadUniform(ArrayName("u_light_pos", i), position);
			d_shader.LoadUniform(ArrayName("u_light_colour", i), light.colour);
			d_shader.LoadUniform(ArrayName("u_light_attenuation", i), light.attenuation);
            d_shader.LoadUniform(ArrayName("u_light_brightness", i), light.brightness);
            ++i;
        }
    });
    while (i < MAX_NUM_LIGHTS) {
        d_shader.LoadUniform(ArrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_attenuation", i), {1.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_brightness", i), 0.0f);
        ++i;
    }

    d_shader.Bind();

    std::string currentModel;
    std::string currentTexture;
    scene.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        const auto& tc = entity.Get<TransformComponent>();
        const auto& mc = entity.Get<ModelComponent>();
        if (mc.model.empty()) { return; }

        bool changedModel = mc.model != currentModel;
        bool changedTexture = mc.texture != currentTexture;

        if (changedModel || changedTexture) {
            d_instanceBuffer->SetData(d_instanceData);
            d_vao->SetInstances(d_instanceBuffer);
            d_vao->Draw();
            d_instanceData.clear();
        }

        if (changedModel) {
            d_vao->SetModel(d_modelManager->GetModel(mc.model));
            currentModel = mc.model;
        }

        if (changedTexture) {
            d_textureManager->GetTexture(mc.texture).Bind();
            currentTexture = mc.texture;
        }

        d_instanceData.push_back({
            tc.position,
            tc.orientation,
            tc.scale,
            mc.shineDamper,
            mc.reflectivity
        });
    });

    d_instanceBuffer->SetData(d_instanceData);
    d_vao->SetInstances(d_instanceBuffer);
    d_vao->Draw();
    d_instanceData.clear();

    if (d_particleManager != nullptr) {
        d_vao->SetModel(d_particleManager->GetModel());
        d_vao->SetInstances(d_particleManager->GetInstances());
        d_vao->Draw();
    }

    d_shader.Unbind();
}

void EntityRenderer::Draw(const Entity& camera, const Lights& lights, Scene& scene)
{
    Maths::mat4 proj = CameraUtils::MakeProj(camera);
    Maths::mat4 view = CameraUtils::MakeView(camera);
    Draw(proj, view, lights, scene);
}

void EntityRenderer::EnableParticles(ParticleManager* particleManager)
{
    d_particleManager = particleManager;
}

}