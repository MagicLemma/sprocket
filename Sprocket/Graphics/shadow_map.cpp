#include "shadow_map.h"

#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Scene/ecs.h>

#include <glad/glad.h>

namespace spkt {

shadow_map::shadow_map(asset_manager* assetManager)
    : d_asset_manager(assetManager)
    , d_shader("Resources/Shaders/ShadowMap.vert", "Resources/Shaders/ShadowMap.frag")
    , d_light_view() // Will be populated after starting a scene.
    , d_light_proj(glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, -50.0f, 50.0f))
    , d_shadow_map(8192, 8192)
{
}

void shadow_map::draw(
    spkt::registry& registry,
    const glm::vec3& sunDirection,
    const glm::vec3& centre)
{
    spkt::render_context rc;
    rc.depth_testing(true);

    // Reduces "peter panning", also stops "flat" objects that dont have a back from
    // casting shadows.
    rc.set_face_cull(GL_FRONT);

    d_light_view = glm::lookAt(centre - sunDirection, centre, {0.0, 1.0, 0.0});

    d_shader.bind();
    d_shader.load("u_proj_matrix", d_light_proj);
    d_shader.load("u_view_matrix", d_light_view);

    d_shadow_map.bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    std::unordered_map<std::string, std::vector<model_instance>> commands;
    for (auto [mc, tc] : registry.view_get<StaticModelComponent, Transform3DComponent>()) {
        commands[mc.mesh].push_back({ tc.position, tc.orientation, tc.scale });
    }

    for (const auto& [key, data] : commands) {
        d_instance_buffer.set_data(data);
        spkt::draw(d_asset_manager->get<static_mesh>(key), &d_instance_buffer);
    }

    d_shadow_map.unbind();
    d_shader.unbind();
}

glm::mat4 shadow_map::get_light_proj_view() const
{
    return d_light_proj * d_light_view;
}

const texture& shadow_map::get_texture() const
{
    return d_shadow_map.depth_texture();
}

}