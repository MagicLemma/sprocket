#include "EntityRenderer.h"
#include "Maths.h"
#include "ModelManager.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Components.h"
#include "Scene.h"
#include "ShadowMap.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

std::shared_ptr<Buffer> GetInstanceBuffer()
{
    BufferLayout layout(sizeof(InstanceData), 5);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 4, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    assert(layout.Validate());

    return std::make_shared<Buffer>(layout, BufferUsage::DYNAMIC);
}

}

EntityRenderer::EntityRenderer(ModelManager* modelManager,
                               MaterialManager* materialManager)
    : d_vao(std::make_unique<VertexArray>())
    , d_modelManager(modelManager)
    , d_materialManager(materialManager)
    , d_particleManager(nullptr)
    //, d_shader("Resources/Shaders/Entity_Classic.vert", "Resources/Shaders/Entity_Classic.frag")
    , d_shader("Resources/Shaders/Entity_PBR.vert", "Resources/Shaders/Entity_PBR.frag")
    , d_instanceBuffer(GetInstanceBuffer())
{
}

void EntityRenderer::EnableShadows(const ShadowMap& shadowMap)
{
    glActiveTexture(GL_TEXTURE4);
    shadowMap.GetShadowMap().Bind();
 
    d_shader.Bind();
    d_shader.LoadSampler("shadow_map", 4);
    d_shader.LoadMat4("u_light_proj_view", shadowMap.GetLightProjViewMatrix());
    glActiveTexture(GL_TEXTURE0);
}

void EntityRenderer::Draw(
    const Maths::mat4& proj,
    const Maths::mat4& view,
    Scene& scene)
{
    RenderContext rc;
    rc.FaceCulling(true);
    rc.DepthTesting(true);

    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.Bind();
    d_shader.LoadMat4("u_proj_matrix", proj);
    d_shader.LoadMat4("u_view_matrix", view);

    // Load sun to shader
    const auto& sun = scene.GetSun();
    d_shader.LoadVec3("u_sun_direction", sun.direction);
    d_shader.LoadVec3("u_sun_colour", sun.colour);
    d_shader.LoadFloat("u_sun_brightness", sun.brightness);

    // Load ambience to shader
    const auto& ambience = scene.GetAmbience();
    d_shader.LoadVec3("u_ambience_colour", ambience.colour);
    d_shader.LoadFloat("u_ambience_brightness", ambience.brightness);
    
    // Load point lights to shader
    std::size_t i = 0;
    scene.Each<TransformComponent, LightComponent>([&](Entity& entity) {
        if (i < MAX_NUM_LIGHTS) {
            auto position = entity.Get<TransformComponent>().position;
            auto light = entity.Get<LightComponent>();
            d_shader.LoadVec3(ArrayName("u_light_pos", i), position);
			d_shader.LoadVec3(ArrayName("u_light_colour", i), light.colour);
            d_shader.LoadFloat(ArrayName("u_light_brightness", i), light.brightness);
            ++i;
        }
    });
    while (i < MAX_NUM_LIGHTS) {
        d_shader.LoadVec3(ArrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
        d_shader.LoadVec3(ArrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
        d_shader.LoadFloat(ArrayName("u_light_brightness", i), 0.0f);
        ++i;
    }

    d_shader.Bind();

    std::string currentMesh = "INVALID";
    std::string currentMaterial = "INVALID";
    // These are set initially to INVALID as that is not a valid file path.
    // They cannot be set to "" as this returns the "default" materials,
    // which causes errors when drawing in some cases. TODO: Find out why,
    // it has something to do with the way ImGui renders things, as the
    // visual errors don't appear when the viewport is not selected, or when
    // the Guizmo is on screen. It is probably writing to the default texture
    // which remains bound. We should fix this properly, as this "fix" causes
    // an extra pointless draw call at the beginning of each frame.

    bool first = true;
    // TODO: Try and remove this flag.
    // On the very first entity, we set both the currentModel and the
    // currentMaterial, so it would issue a draw call. This would lead to a
    // seg fault as no mesh or material would be set. This is a latent bug;
    // it was being masked by a previous model still being loaded, and was
    // revealed when moving skybox loading code.
    
    scene.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        const auto& tc = entity.Get<TransformComponent>();
        const auto& mc = entity.Get<ModelComponent>();
        if (mc.mesh.empty()) { return; }

        bool changedMesh = mc.mesh != currentMesh;
        bool changedMaterial = mc.material != currentMaterial;

        if (changedMesh || changedMaterial) {
            if (!first) {
                d_instanceBuffer->SetData(d_instanceData);
                d_vao->SetInstances(d_instanceBuffer);
                d_vao->Draw();
                d_instanceData.clear();
            }
            first = false;
        }

        if (changedMesh) {
            d_vao->SetModel(d_modelManager->GetModel(mc.mesh));
            currentMesh = mc.mesh;
        }

        if (changedMaterial) {
            auto material = d_materialManager->GetMaterial(mc.material);
            // TODO: Apply everything

            glActiveTexture(GL_TEXTURE0);
            material->albedoMap->Bind();
            d_shader.LoadSampler("texture_sampler", 0);

            glActiveTexture(GL_TEXTURE1);
            material->normalMap->Bind();
            d_shader.LoadSampler("u_normal_map", 1);
            
            glActiveTexture(GL_TEXTURE2);
            material->metallicMap->Bind();
            d_shader.LoadSampler("u_metallic_map", 2);

            glActiveTexture(GL_TEXTURE3);
            material->roughnessMap->Bind();
            d_shader.LoadSampler("u_roughness_map", 3);

            glActiveTexture(GL_TEXTURE0);

            d_shader.LoadFloat("u_use_albedo_map", material->useAlbedoMap ? 1.0f : 0.0f);
            d_shader.LoadFloat("u_use_normal_map", material->useNormalMap ? 1.0f : 0.0f);
            d_shader.LoadFloat("u_use_metallic_map", material->useMetallicMap ? 1.0f : 0.0f);
            d_shader.LoadFloat("u_use_roughness_map", material->useRoughnessMap ? 1.0f : 0.0f);

            // u_albedo
            d_shader.LoadVec3("u_albedo", material->albedo);
            d_shader.LoadFloat("u_roughness", material->roughness);
            d_shader.LoadFloat("u_metallic", material->metallic);
            currentMaterial = mc.material;
        }

        d_instanceData.push_back({ tc.position, tc.orientation, tc.scale });
    });

    d_instanceBuffer->SetData(d_instanceData);
    d_vao->SetInstances(d_instanceBuffer);
    d_vao->Draw();
    d_instanceData.clear();

    if (d_particleManager != nullptr) {
        // TODO: Un-hardcode this, do when cleaning up the rendering.
        d_vao->SetModel(d_modelManager->GetModel("Resources/Models/Particle.obj"));
        d_vao->SetInstances(d_particleManager->GetInstances());
        d_vao->Draw();
    }

    d_shader.Unbind();
}

void EntityRenderer::Draw(const Entity& camera, Scene& scene)
{
    Maths::mat4 proj = MakeProj(camera);
    Maths::mat4 view = MakeView(camera);
    Draw(proj, view, scene);
}

void EntityRenderer::EnableParticles(ParticleManager* particleManager)
{
    d_particleManager = particleManager;
}

}