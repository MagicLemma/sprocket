#include "Scene3DRenderer.h"

#include <Sprocket/Graphics/AssetManager.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Scene/Camera.h>
#include <Sprocket/Utility/Hashing.h>
#include <Sprocket/Utility/Maths.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>

namespace spkt {
namespace {

std::array<glm::mat4, Scene3DRenderer::MAX_BONES> DefaultBoneTransforms() {
    std::array<glm::mat4, Scene3DRenderer::MAX_BONES> arr;
    std::ranges::fill(arr, glm::mat4(1.0));
    return arr;
};

void upload_uniforms(
    const shader& shader,
    const spkt::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view
)
{
    std::uint32_t MAX_NUM_LIGHTS = 5;
      shader.bind();

    shader.load("u_proj_matrix", proj);
    shader.load("u_view_matrix", view);

    // Load sun to shader
    if (auto s = registry.find<SunComponent>(); registry.valid(s)) {
        const auto& sun = registry.get<SunComponent>(s);
        shader.load("u_sun_direction", sun.direction);
        shader.load("u_sun_colour", sun.colour);
        shader.load("u_sun_brightness", sun.brightness);
    }

    // Load ambience to shader
    if (auto a = registry.find<AmbienceComponent>(); registry.valid(a)) {
        const auto& ambience = registry.get<AmbienceComponent>(a);
        shader.load("u_ambience_colour", ambience.colour);
        shader.load("u_ambience_brightness", ambience.brightness);
    }
    
    // Load point lights to shader
    std::size_t i = 0;
    for (auto entity : registry.view<LightComponent, Transform3DComponent>()) {
        if (i < MAX_NUM_LIGHTS) {
            auto position = registry.get<Transform3DComponent>(entity).position;
            auto light = registry.get<LightComponent>(entity);
            shader.load(array_name("u_light_pos", i), position);
            shader.load(array_name("u_light_colour", i), light.colour);
            shader.load(array_name("u_light_brightness", i), light.brightness);
            ++i;
        }
        else {
            break;
        }
    }
    while (i < MAX_NUM_LIGHTS) {
        shader.load(array_name("u_light_pos", i), {0.0f, 0.0f, 0.0f});
        shader.load(array_name("u_light_colour", i), {0.0f, 0.0f, 0.0f});
        shader.load(array_name("u_light_brightness", i), 0.0f);
        ++i;
    }
}

void UploadMaterial(
    const shader& shader,
    Material* material,
    AssetManager* assetManager
)
{
    assetManager->GetTexture(material->albedoMap)->Bind(Scene3DRenderer::ALBEDO_SLOT);
    assetManager->GetTexture(material->normalMap)->Bind(Scene3DRenderer::NORMAL_SLOT);
    assetManager->GetTexture(material->metallicMap)->Bind(Scene3DRenderer::METALLIC_SLOT);
    assetManager->GetTexture(material->roughnessMap)->Bind(Scene3DRenderer::ROUGHNESS_SLOT);

    shader.load("u_use_albedo_map", material->useAlbedoMap ? 1.0f : 0.0f);
    shader.load("u_use_normal_map", material->useNormalMap ? 1.0f : 0.0f);
    shader.load("u_use_metallic_map", material->useMetallicMap ? 1.0f : 0.0f);
    shader.load("u_use_roughness_map", material->useRoughnessMap ? 1.0f : 0.0f);

    shader.load("u_albedo", material->albedo);
    shader.load("u_roughness", material->roughness);
    shader.load("u_metallic", material->metallic);
}

}

Scene3DRenderer::Scene3DRenderer(AssetManager* assetManager)
    : d_assetManager(assetManager)
    , d_staticShader("Resources/Shaders/Entity_PBR_Static.vert", "Resources/Shaders/Entity_PBR.frag")
    , d_animatedShader("Resources/Shaders/Entity_PBR_Animated.vert", "Resources/Shaders/Entity_PBR.frag")
    , d_instanceBuffer()
{
    d_staticShader.bind();
    d_staticShader.load("u_albedo_map", ALBEDO_SLOT);
    d_staticShader.load("u_normal_map", NORMAL_SLOT);
    d_staticShader.load("u_metallic_map", METALLIC_SLOT);
    d_staticShader.load("u_roughness_map", ROUGHNESS_SLOT);
    d_staticShader.unbind();

    d_animatedShader.bind();
    d_animatedShader.load("u_albedo_map", ALBEDO_SLOT);
    d_animatedShader.load("u_normal_map", NORMAL_SLOT);
    d_animatedShader.load("u_metallic_map", METALLIC_SLOT);
    d_animatedShader.load("u_roughness_map", ROUGHNESS_SLOT);
    d_animatedShader.unbind();
}

void Scene3DRenderer::EnableShadows(const ShadowMap& shadowMap)
{
    d_staticShader.bind();
    d_staticShader.load("shadow_map", SHADOW_MAP_SLOT);
    d_staticShader.load("u_light_proj_view", shadowMap.GetLightProjViewMatrix());
    d_staticShader.unbind();

    d_animatedShader.bind();
    d_animatedShader.load("shadow_map", SHADOW_MAP_SLOT);
    d_animatedShader.load("u_light_proj_view", shadowMap.GetLightProjViewMatrix());
    d_animatedShader.unbind();
 
    shadowMap.GetShadowMap()->Bind(SHADOW_MAP_SLOT);
}

void Scene3DRenderer::Draw(
    const spkt::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view)
{
    spkt::render_context rc;
    rc.face_culling(true);
    rc.depth_testing(true);

    upload_uniforms(d_staticShader, registry, proj, view);
    upload_uniforms(d_animatedShader, registry, proj, view);

    std::unordered_map<
        std::pair<std::string, std::string>,
        std::vector<spkt::model_instance>,
        spkt::hash_pair
    > commands;

    d_staticShader.bind();
    for (auto entity : registry.view<StaticModelComponent, Transform3DComponent>()) {
        const auto& tc = registry.get<Transform3DComponent>(entity);
        const auto& mc = registry.get<StaticModelComponent>(entity);
        commands[{ mc.mesh, mc.material }].push_back({ tc.position, tc.orientation, tc.scale });
    }

    for (const auto& [key, data] : commands) {
        auto mesh = d_assetManager->get_static_mesh(key.first);
        auto material = d_assetManager->GetMaterial(key.second);

        UploadMaterial(d_staticShader, material, d_assetManager);
        d_instanceBuffer.set_data(data);
        spkt::draw(mesh, &d_instanceBuffer);
    }

    // If the scene has a ParticleSingleton, then render the particles that it contains.
    for (auto entity : registry.view<ParticleSingleton>()) {
        const auto& ps = registry.get<ParticleSingleton>(entity);
        std::vector<spkt::model_instance> instance_data(NUM_PARTICLES);
        for (const auto& particle : *ps.particles) {
            if (particle.life > 0.0) {
                instance_data.push_back({
                    particle.position,
                    {0.0, 0.0, 0.0, 1.0},
                    particle.scale
                });
            }
        }
        d_instanceBuffer.set_data(instance_data);

        // TODO: Un-hardcode this mesh, do when cleaning up the rendering.
        spkt::draw(d_assetManager->get_static_mesh("Resources/Models/Particle.obj"), &d_instanceBuffer);
    }

    d_animatedShader.bind();
    for (auto entity : registry.view<AnimatedModelComponent, Transform3DComponent>()) {
        const auto& tc = registry.get<Transform3DComponent>(entity);
        const auto& mc = registry.get<AnimatedModelComponent>(entity);
        auto mesh = d_assetManager->get_animated_mesh(mc.mesh);

        auto material = d_assetManager->GetMaterial(mc.material);
        UploadMaterial(d_animatedShader, material, d_assetManager);

        d_animatedShader.load("u_model_matrix", Maths::Transform(tc.position, tc.orientation, tc.scale));
        
        auto poses = mesh->get_pose(mc.animation_name, mc.animation_time);
        poses.resize(MAX_BONES, glm::mat4(1.0));
        d_animatedShader.load("u_bone_transforms", poses[0], MAX_BONES);

        spkt::draw(mesh);
        
    }
    d_animatedShader.unbind();
}

void Scene3DRenderer::Draw(const spkt::registry& registry, spkt::entity camera)
{
    glm::mat4 proj = spkt::make_proj(registry, camera);
    glm::mat4 view = spkt::make_view(registry, camera);
    Draw(registry, proj, view);
}

}