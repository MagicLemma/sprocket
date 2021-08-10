#include "Scene3DRenderer.h"

#include <Sprocket/Graphics/AssetManager.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Scene/camera.h>
#include <Sprocket/Utility/Hashing.h>
#include <Sprocket/Utility/Maths.h>
#include <Sprocket/Utility/views.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>

namespace spkt {
namespace {

std::array<glm::mat4, MAX_BONES> DefaultBoneTransforms() {
    std::array<glm::mat4, MAX_BONES> arr;
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
    std::array<glm::vec3, MAX_NUM_LIGHTS> positions = {};
    std::array<glm::vec3, MAX_NUM_LIGHTS> colours = {};
    std::array<float, MAX_NUM_LIGHTS> brightnesses = {};

    for (auto [index, data] : registry.view_get<LightComponent, Transform3DComponent>()
                            | std::views::take(MAX_NUM_LIGHTS)
                            | spkt::views::enumerate())
    {
        auto [light, transform] = data;
        positions[index] = transform.position;
        colours[index] = light.colour;
        brightnesses[index] = light.brightness;
    }

    shader.load("u_light_pos", positions);
    shader.load("u_light_colour", colours);
    shader.load("u_light_brightness", brightnesses);
}

void UploadMaterial(
    const shader& shader,
    Material* material,
    AssetManager* assetManager
)
{
    assetManager->GetTexture(material->albedoMap)->Bind(ALBEDO_SLOT);
    assetManager->GetTexture(material->normalMap)->Bind(NORMAL_SLOT);
    assetManager->GetTexture(material->metallicMap)->Bind(METALLIC_SLOT);
    assetManager->GetTexture(material->roughnessMap)->Bind(ROUGHNESS_SLOT);

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
    d_staticShader.load("u_albedo_map", ALBEDO_SLOT);
    d_staticShader.load("u_normal_map", NORMAL_SLOT);
    d_staticShader.load("u_metallic_map", METALLIC_SLOT);
    d_staticShader.load("u_roughness_map", ROUGHNESS_SLOT);
    d_staticShader.load("shadow_map", SHADOW_MAP_SLOT);

    d_animatedShader.load("u_albedo_map", ALBEDO_SLOT);
    d_animatedShader.load("u_normal_map", NORMAL_SLOT);
    d_animatedShader.load("u_metallic_map", METALLIC_SLOT);
    d_animatedShader.load("u_roughness_map", ROUGHNESS_SLOT);
    d_animatedShader.load("shadow_map", SHADOW_MAP_SLOT);
}

void Scene3DRenderer::EnableShadows(const ShadowMap& shadowMap)
{
    d_staticShader.load("u_light_proj_view", shadowMap.GetLightProjViewMatrix());
    d_animatedShader.load("u_light_proj_view", shadowMap.GetLightProjViewMatrix());
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
    for (auto [mc, tc] : registry.view_get<StaticModelComponent, Transform3DComponent>()) {
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
    for (auto [ps] : registry.view_get<ParticleSingleton>()) {
        std::vector<spkt::model_instance> instance_data(NUM_PARTICLES);
        for (const auto& particle : *ps.particles) {
            if (particle.life > 0.0) {
                instance_data.push_back({particle.position, {0.0, 0.0, 0.0, 1.0}, particle.scale});
            }
        }
        d_instanceBuffer.set_data(instance_data);

        // TODO: Un-hardcode this mesh, do when cleaning up the rendering.
        spkt::draw(d_assetManager->get_static_mesh("Resources/Models/Particle.obj"), &d_instanceBuffer);
    }

    d_animatedShader.bind();
    for (auto [mc, tc] : registry.view_get<AnimatedModelComponent, Transform3DComponent>()) {
        auto mesh = d_assetManager->get_animated_mesh(mc.mesh);
        auto material = d_assetManager->GetMaterial(mc.material);
        UploadMaterial(d_animatedShader, material, d_assetManager);

        d_animatedShader.load("u_model_matrix", Maths::Transform(tc.position, tc.orientation, tc.scale));
        
        auto poses = mesh->get_pose(mc.animation_name, mc.animation_time);
        poses.resize(MAX_BONES, glm::mat4(1.0));
        d_animatedShader.load("u_bone_transforms", poses);

        spkt::draw(mesh);
        
    }
}

void Scene3DRenderer::Draw(const spkt::registry& registry, spkt::entity camera)
{
    glm::mat4 proj = spkt::make_proj(registry, camera);
    glm::mat4 view = spkt::make_view(registry, camera);
    Draw(registry, proj, view);
}

}