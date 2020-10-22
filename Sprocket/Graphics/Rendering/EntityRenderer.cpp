#include "EntityRenderer.h"
#include "Maths.h"
#include "AssetManager.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Components.h"
#include "Scene.h"
#include "ShadowMap.h"
#include "Types.h"

#include <glad/glad.h>
#include <cmath>

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

void UploadUniforms(
    const Shader& shader,
    const Maths::mat4& proj,
    const Maths::mat4& view,
    Scene& scene)
{
    u32 MAX_NUM_LIGHTS = 5;
    shader.Bind();
    shader.LoadMat4("u_proj_matrix", proj);
    shader.LoadMat4("u_view_matrix", view);

    // Load sun to shader
    const auto& sun = scene.GetSun();
    shader.LoadVec3("u_sun_direction", sun.direction);
    shader.LoadVec3("u_sun_colour", sun.colour);
    shader.LoadFloat("u_sun_brightness", sun.brightness);

    // Load ambience to shader
    const auto& ambience = scene.GetAmbience();
    shader.LoadVec3("u_ambience_colour", ambience.colour);
    shader.LoadFloat("u_ambience_brightness", ambience.brightness);
    
    // Load point lights to shader
    std::size_t i = 0;
    scene.Each<TransformComponent, LightComponent>([&](Entity& entity) {
        if (i < MAX_NUM_LIGHTS) {
            auto position = entity.Get<TransformComponent>().position;
            auto light = entity.Get<LightComponent>();
            shader.LoadVec3(ArrayName("u_light_pos", i), position);
			shader.LoadVec3(ArrayName("u_light_colour", i), light.colour);
            shader.LoadFloat(ArrayName("u_light_brightness", i), light.brightness);
            ++i;
        }
    });
    while (i < MAX_NUM_LIGHTS) {
        shader.LoadVec3(ArrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
        shader.LoadVec3(ArrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
        shader.LoadFloat(ArrayName("u_light_brightness", i), 0.0f);
        ++i;
    }
}

}

EntityRenderer::EntityRenderer(AssetManager* assetManager)
    : d_vao(std::make_unique<VertexArray>())
    , d_assetManager(assetManager)
    , d_particleManager(nullptr)
    , d_staticShader("Resources/Shaders/Entity_PBR_Static.vert", "Resources/Shaders/Entity_PBR.frag")
    , d_animatedShader("Resources/Shaders/Entity_PBR_Animated.vert", "Resources/Shaders/Entity_PBR.frag")
    , d_instanceBuffer(GetInstanceBuffer())
{
    d_staticShader.Bind();
    d_staticShader.LoadSampler("u_albedo_map", ALBEDO_SLOT);
    d_staticShader.LoadSampler("u_normal_map", NORMAL_SLOT);
    d_staticShader.LoadSampler("u_metallic_map", METALLIC_SLOT);
    d_staticShader.LoadSampler("u_roughness_map", ROUGHNESS_SLOT);
    d_staticShader.Unbind();

    d_animatedShader.Bind();
    d_animatedShader.LoadSampler("u_albedo_map", ALBEDO_SLOT);
    d_animatedShader.LoadSampler("u_normal_map", NORMAL_SLOT);
    d_animatedShader.LoadSampler("u_metallic_map", METALLIC_SLOT);
    d_animatedShader.LoadSampler("u_roughness_map", ROUGHNESS_SLOT);
    d_animatedShader.Unbind();
}

void EntityRenderer::EnableShadows(const ShadowMap& shadowMap)
{
    d_staticShader.Bind();
    d_staticShader.LoadSampler("shadow_map", SHADOW_MAP_SLOT);
    d_staticShader.LoadMat4("u_light_proj_view", shadowMap.GetLightProjViewMatrix());
    d_staticShader.Unbind();

    d_animatedShader.Bind();
    d_animatedShader.LoadSampler("shadow_map", SHADOW_MAP_SLOT);
    d_animatedShader.LoadMat4("u_light_proj_view", shadowMap.GetLightProjViewMatrix());
    d_animatedShader.Unbind();
 
    shadowMap.GetShadowMap()->Bind(SHADOW_MAP_SLOT);
}

void EntityRenderer::Draw(
    const Maths::mat4& proj,
    const Maths::mat4& view,
    Scene& scene)
{
    RenderContext rc;
    rc.FaceCulling(true);
    rc.DepthTesting(true);

    UploadUniforms(d_staticShader, proj, view, scene);
    UploadUniforms(d_animatedShader, proj, view, scene);

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
    
    d_staticShader.Bind();
    scene.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        const auto& tc = entity.Get<TransformComponent>();
        const auto& mc = entity.Get<ModelComponent>();
        if (mc.mesh.empty()) { return; }
        auto mesh = d_assetManager->GetMesh(mc.mesh);
        if (mesh->IsAnimated()) { return; }

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
            d_vao->SetModel(mesh);
            currentMesh = mc.mesh;
        }

        if (changedMaterial) {
            auto material = d_assetManager->GetMaterial(mc.material);
            currentMaterial = mc.material;

            d_assetManager->GetTexture(material->albedoMap)->Bind(ALBEDO_SLOT);
            d_assetManager->GetTexture(material->normalMap)->Bind(NORMAL_SLOT);
            d_assetManager->GetTexture(material->metallicMap)->Bind(METALLIC_SLOT);
            d_assetManager->GetTexture(material->roughnessMap)->Bind(ROUGHNESS_SLOT);

            d_staticShader.LoadFloat("u_use_albedo_map", material->useAlbedoMap ? 1.0f : 0.0f);
            d_staticShader.LoadFloat("u_use_normal_map", material->useNormalMap ? 1.0f : 0.0f);
            d_staticShader.LoadFloat("u_use_metallic_map", material->useMetallicMap ? 1.0f : 0.0f);
            d_staticShader.LoadFloat("u_use_roughness_map", material->useRoughnessMap ? 1.0f : 0.0f);

            d_staticShader.LoadVec3("u_albedo", material->albedo);
            d_staticShader.LoadFloat("u_roughness", material->roughness);
            d_staticShader.LoadFloat("u_metallic", material->metallic);
        }

        d_instanceData.push_back({ tc.position, tc.orientation, tc.scale });
    });

    d_instanceBuffer->SetData(d_instanceData);
    d_vao->SetInstances(d_instanceBuffer);
    d_vao->Draw();
    d_instanceData.clear();

    if (d_particleManager != nullptr) {
        // TODO: Un-hardcode this, do when cleaning up the rendering.
        d_vao->SetModel(d_assetManager->GetMesh("Resources/Models/Particle.obj"));
        d_vao->SetInstances(d_particleManager->GetInstances());
        d_vao->Draw();
    }

    d_animatedShader.Bind();
    scene.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        const auto& tc = entity.Get<TransformComponent>();
        const auto& mc = entity.Get<ModelComponent>();
        if (mc.mesh.empty()) { return; }
        auto mesh = d_assetManager->GetMesh(mc.mesh);
        if (!mesh->IsAnimated()) { return; }

        auto material = d_assetManager->GetMaterial(mc.material);
        d_assetManager->GetTexture(material->albedoMap)->Bind(ALBEDO_SLOT);
        d_assetManager->GetTexture(material->normalMap)->Bind(NORMAL_SLOT);
        d_assetManager->GetTexture(material->metallicMap)->Bind(METALLIC_SLOT);
        d_assetManager->GetTexture(material->roughnessMap)->Bind(ROUGHNESS_SLOT);

        d_animatedShader.LoadFloat("u_use_albedo_map", material->useAlbedoMap ? 1.0f : 0.0f);
        d_animatedShader.LoadFloat("u_use_normal_map", material->useNormalMap ? 1.0f : 0.0f);
        d_animatedShader.LoadFloat("u_use_metallic_map", material->useMetallicMap ? 1.0f : 0.0f);
        d_animatedShader.LoadFloat("u_use_roughness_map", material->useRoughnessMap ? 1.0f : 0.0f);

        d_animatedShader.LoadVec3("u_albedo", material->albedo);
        d_animatedShader.LoadFloat("u_roughness", material->roughness);
        d_animatedShader.LoadFloat("u_metallic", material->metallic);

        d_animatedShader.LoadMat4("u_model_matrix", Maths::Transform(tc.position, tc.orientation, tc.scale));
        
        if (entity.Has<AnimationComponent>()) {
            const auto& ac = entity.Get<AnimationComponent>();
            auto poses = mesh->GetPose(ac.name, ac.time);
            
            int numBones = std::min(MAX_BONES, (int)poses.size());
            d_animatedShader.LoadMat4("u_bone_transforms", poses[0], numBones);
        }
        else {
            static const auto clear = []() {
                std::array<Maths::mat4, MAX_BONES> arr;
                for (auto& x : arr) { x = Maths::mat4(1.0); }
                return arr;
            }();
            d_animatedShader.LoadMat4("u_bone_transforms", clear[0], MAX_BONES);
        }

        d_vao->SetModel(mesh);
        d_vao->SetInstances(nullptr);
        d_vao->Draw();
    });
    d_animatedShader.Unbind();
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