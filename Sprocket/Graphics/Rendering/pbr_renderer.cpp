#include "pbr_renderer.h"

#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/camera.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/render_context.h>
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

void upload_material(
    const shader& shader,
    const material& material,
    asset_manager* assetManager
)
{
    assetManager->get<texture>(material.albedoMap).bind(ALBEDO_SLOT);
    assetManager->get<texture>(material.normalMap).bind(NORMAL_SLOT);
    assetManager->get<texture>(material.metallicMap).bind(METALLIC_SLOT);
    assetManager->get<texture>(material.roughnessMap).bind(ROUGHNESS_SLOT);

    shader.load("u_use_albedo_map", material.useAlbedoMap ? 1.0f : 0.0f);
    shader.load("u_use_normal_map", material.useNormalMap ? 1.0f : 0.0f);
    shader.load("u_use_metallic_map", material.useMetallicMap ? 1.0f : 0.0f);
    shader.load("u_use_roughness_map", material.useRoughnessMap ? 1.0f : 0.0f);

    shader.load("u_albedo", material.albedo);
    shader.load("u_roughness", material.roughness);
    shader.load("u_metallic", material.metallic);
}

}

pbr_renderer::pbr_renderer(asset_manager* assetManager)
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

void pbr_renderer::enable_shadows(const shadow_map& shadowMap)
{
    d_staticShader.load("u_light_proj_view", shadowMap.get_light_proj_view());
    d_animatedShader.load("u_light_proj_view", shadowMap.get_light_proj_view());
    shadowMap.get_texture().bind(SHADOW_MAP_SLOT);
}

void pbr_renderer::for_each_shader(const std::function<void(spkt::shader& shader)>& callback)
{
    callback(d_staticShader);
    callback(d_animatedShader);
}

void pbr_renderer::begin_frame(const glm::mat4& proj, const glm::mat4& view)
{
    assert(!d_frame_data);
    d_frame_data = frame_data{};
    for_each_shader([&](spkt::shader& shader) {
        shader.bind();
        shader.load("u_proj_matrix", proj);
        shader.load("u_view_matrix", view);
    });
}

void pbr_renderer::end_frame()
{
    assert(d_frame_data);
    for_each_shader([&](spkt::shader& shader) {
        shader.bind();
        shader.load("u_light_pos", d_frame_data->light_positions);
        shader.load("u_light_colour",  d_frame_data->light_colours);
        shader.load("u_light_brightness",  d_frame_data->light_brightnesses);
    });

    d_staticShader.bind();
    for (const auto& [key, data] : d_frame_data->static_mesh_draw_commands) {
        const auto& mesh = d_assetManager->get<static_mesh>(key.first);
        const auto& mat = d_assetManager->get<material>(key.second);

        upload_material(d_staticShader, mat, d_assetManager);
        d_instanceBuffer.set_data(data);
        spkt::draw(mesh, &d_instanceBuffer);
    }
    d_staticShader.unbind();

    d_frame_data = std::nullopt;
}

void pbr_renderer::set_ambience(const glm::vec3& colour, const float brightness)
{
    for_each_shader([&](spkt::shader& shader) {
        shader.bind();
        shader.load("u_ambience_colour", colour);
        shader.load("u_ambience_brightness", brightness);
    });
}

void pbr_renderer::set_sunlight(
    const glm::vec3& colour, const glm::vec3& direction, const float brightness)
{
    for_each_shader([&](spkt::shader& shader) {
        shader.bind();
        shader.load("u_sun_colour", colour);
        shader.load("u_sun_direction", direction);
        shader.load("u_sun_brightness", brightness);
    });
}

void pbr_renderer::add_light(
    const glm::vec3& position, const glm::vec3& colour, const float brightness)
{
    assert(d_frame_data);
    auto& index = d_frame_data->next_light_index;
    if (index == spkt::MAX_NUM_LIGHTS) { return; }

    d_frame_data->light_positions[index] = position;
    d_frame_data->light_colours[index] = colour;
    d_frame_data->light_brightnesses[index] = brightness;
    ++index;
}

void pbr_renderer::draw_static_mesh(
    const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale,
    const std::string& mesh, const std::string& material)
{
    assert(d_frame_data);
    d_frame_data->static_mesh_draw_commands[{mesh, material}].push_back({position, orientation, scale});
}

void pbr_renderer::draw_animated_mesh(
    const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale,
    const std::string& mesh, const std::string& material,
    const std::string& animation_name, float animation_time)
{
    assert(d_frame_data);
    d_animatedShader.bind();
    const auto& mesh_obj = d_assetManager->get<animated_mesh>(mesh);
    const auto& mat = d_assetManager->get<spkt::material>(material);
    upload_material(d_animatedShader, mat, d_assetManager);

    d_animatedShader.load("u_model_matrix", Maths::Transform(position, orientation, scale));
    
    auto poses = mesh_obj.get_pose(animation_name, animation_time);
    poses.resize(MAX_BONES, glm::mat4(1.0));
    d_animatedShader.load("u_bone_transforms", poses);

    spkt::draw(mesh_obj);
    d_animatedShader.unbind();
}

void pbr_renderer::draw_particles(std::span<const spkt::model_instance> particles)
{
    d_staticShader.bind();
    d_instanceBuffer.set_data(particles);

    // TODO: Un-hardcode this mesh, do when cleaning up the rendering.
    spkt::draw(d_assetManager->get<static_mesh>("Resources/Models/Particle.obj"), &d_instanceBuffer);
    d_staticShader.unbind();
}

}