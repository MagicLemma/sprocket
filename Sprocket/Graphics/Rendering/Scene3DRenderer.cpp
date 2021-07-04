#include "Scene3DRenderer.h"
#include "Maths.h"
#include "AssetManager.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Components.h"
#include "Scene.h"
#include "ShadowMap.h"
#include "Types.h"
#include "Hashing.h"

#include "apecs.hpp"

#include <algorithm>
#include <ranges>

namespace spkt {
namespace {

std::array<glm::mat4, Scene3DRenderer::MAX_BONES> DefaultBoneTransforms() {
    std::array<glm::mat4, Scene3DRenderer::MAX_BONES> arr;
    std::ranges::fill(arr, glm::mat4(1.0));
    return arr;
};

std::unique_ptr<Buffer> GetInstanceBuffer()
{
    BufferLayout layout(sizeof(InstanceData), 5);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 4, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    assert(layout.Validate());

    return std::make_unique<Buffer>(layout, BufferUsage::DYNAMIC);
}

void UploadUniforms(
    const Shader& shader,
    const glm::mat4& proj,
    const glm::mat4& view,
    Scene& scene
)
{
    u32 MAX_NUM_LIGHTS = 5;
    shader.Bind();
    shader.LoadMat4("u_proj_matrix", proj);
    shader.LoadMat4("u_view_matrix", view);

    // Load sun to shader
    if (auto s = scene.find<SunComponent>(); s.valid()) {
        const auto& sun = s.get<SunComponent>();
        shader.LoadVec3("u_sun_direction", sun.direction);
        shader.LoadVec3("u_sun_colour", sun.colour);
        shader.LoadFloat("u_sun_brightness", sun.brightness);
    }

    // Load ambience to shader
    if (auto a = scene.find<AmbienceComponent>(); a.valid()) {
        const auto& ambience = a.get<AmbienceComponent>();
        shader.LoadVec3("u_ambience_colour", ambience.colour);
        shader.LoadFloat("u_ambience_brightness", ambience.brightness);
    }
    
    // Load point lights to shader
    std::size_t i = 0;
    for (auto entity : scene.view<LightComponent, Transform3DComponent>()) {
        if (i < MAX_NUM_LIGHTS) {
            auto position = entity.get<Transform3DComponent>().position;
            auto light = entity.get<LightComponent>();
            shader.LoadVec3(ArrayName("u_light_pos", i), position);
            shader.LoadVec3(ArrayName("u_light_colour", i), light.colour);
            shader.LoadFloat(ArrayName("u_light_brightness", i), light.brightness);
            ++i;
        }
        else {
            break;
        }
    }
    while (i < MAX_NUM_LIGHTS) {
        shader.LoadVec3(ArrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
        shader.LoadVec3(ArrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
        shader.LoadFloat(ArrayName("u_light_brightness", i), 0.0f);
        ++i;
    }
}

void UploadMaterial(
    const Shader& shader,
    Material* material,
    AssetManager* assetManager
)
{
    assetManager->GetTexture(material->albedoMap)->Bind(Scene3DRenderer::ALBEDO_SLOT);
    assetManager->GetTexture(material->normalMap)->Bind(Scene3DRenderer::NORMAL_SLOT);
    assetManager->GetTexture(material->metallicMap)->Bind(Scene3DRenderer::METALLIC_SLOT);
    assetManager->GetTexture(material->roughnessMap)->Bind(Scene3DRenderer::ROUGHNESS_SLOT);

    shader.LoadFloat("u_use_albedo_map", material->useAlbedoMap ? 1.0f : 0.0f);
    shader.LoadFloat("u_use_normal_map", material->useNormalMap ? 1.0f : 0.0f);
    shader.LoadFloat("u_use_metallic_map", material->useMetallicMap ? 1.0f : 0.0f);
    shader.LoadFloat("u_use_roughness_map", material->useRoughnessMap ? 1.0f : 0.0f);

    shader.LoadVec3("u_albedo", material->albedo);
    shader.LoadFloat("u_roughness", material->roughness);
    shader.LoadFloat("u_metallic", material->metallic);
}

}

Scene3DRenderer::Scene3DRenderer(AssetManager* assetManager)
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

void Scene3DRenderer::EnableShadows(const ShadowMap& shadowMap)
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

void Scene3DRenderer::Draw(
    const glm::mat4& proj,
    const glm::mat4& view,
    Scene& scene)
{
    RenderContext rc;
    rc.FaceCulling(true);
    rc.DepthTesting(true);

    UploadUniforms(d_staticShader, proj, view, scene);
    UploadUniforms(d_animatedShader, proj, view, scene);

    std::unordered_map<
        std::pair<std::string, std::string>,
        std::vector<InstanceData>,
        spkt::hash_pair
    > commands;

    d_staticShader.Bind();
    for (auto entity : scene.view<ModelComponent, Transform3DComponent>()) {
        const auto& tc = entity.get<Transform3DComponent>();
        const auto& mc = entity.get<ModelComponent>();
        if (mc.mesh.empty()) { continue; }
        auto mesh = d_assetManager->GetMesh(mc.mesh);
        if (mesh->IsAnimated()) { continue; }
        commands[{ mc.mesh, mc.material }].push_back({ tc.position, tc.orientation, tc.scale });
    }

    for (const auto& [key, data] : commands) {
        auto mesh = d_assetManager->GetMesh(key.first);
        auto material = d_assetManager->GetMaterial(key.second);
        if (!mesh || !material) { continue; }

        UploadMaterial(d_staticShader, material, d_assetManager);
        d_vao->SetModel(mesh);
        d_instanceBuffer->SetData(data);
        d_vao->SetInstances(d_instanceBuffer.get());
        d_vao->Draw();
    }

    if (d_particleManager != nullptr) {
        // TODO: Un-hardcode this, do when cleaning up the rendering.
        d_vao->SetModel(d_assetManager->GetMesh("Resources/Models/Particle.obj"));
        d_vao->SetInstances(d_particleManager->GetInstances());
        d_vao->Draw();
    }

    d_animatedShader.Bind();
    for (auto entity : scene.view<ModelComponent>()) {
        const auto& tc = entity.get<Transform3DComponent>();
        const auto& mc = entity.get<ModelComponent>();
        if (mc.mesh.empty()) { continue; }
        auto mesh = d_assetManager->GetMesh(mc.mesh);
        if (!mesh->IsAnimated()) { continue; }

        auto material = d_assetManager->GetMaterial(mc.material);
        UploadMaterial(d_animatedShader, material, d_assetManager);

        d_animatedShader.LoadMat4("u_model_matrix", Maths::Transform(tc.position, tc.orientation, tc.scale));
        
        if (entity.has<MeshAnimationComponent>()) {
            const auto& ac = entity.get<MeshAnimationComponent>();
            auto poses = mesh->GetPose(ac.name, ac.time);
            
            int numBones = std::min(MAX_BONES, (int)poses.size());
            d_animatedShader.LoadMat4("u_bone_transforms", poses[0], numBones);
        }
        else {
            static const std::array<glm::mat4, MAX_BONES> clear = DefaultBoneTransforms();
            d_animatedShader.LoadMat4("u_bone_transforms", clear[0], MAX_BONES);
        }

        d_vao->SetModel(mesh);
        d_vao->SetInstances(nullptr);
        d_vao->Draw();
    }
    d_animatedShader.Unbind();
}

void Scene3DRenderer::Draw(apx::handle camera, Scene& scene)
{
    glm::mat4 proj = spkt::make_proj(camera);
    glm::mat4 view = spkt::make_view(camera);
    Draw(proj, view, scene);
}

void Scene3DRenderer::EnableParticles(ParticleManager* particleManager)
{
    d_particleManager = particleManager;
}

}