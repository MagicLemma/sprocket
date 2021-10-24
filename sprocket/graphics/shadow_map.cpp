#include "shadow_map.h"

#include <sprocket/graphics/render_context.h>

#include <glad/glad.h>

namespace spkt {

shadow_map::shadow_map(asset_manager* assetManager)
    : d_asset_manager(assetManager)
    , d_shader("Resources/Shaders/ShadowMap.vert", "Resources/Shaders/ShadowMap.frag")
    , d_light_view() // Will be populated after starting a scene.
    , d_light_proj(glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, -50.0f, 50.0f))
    , d_shadow_map(8192, 8192)
{
    d_shader.bind();
    d_shader.load("u_proj_matrix", d_light_proj);
}

glm::mat4 shadow_map::get_light_proj_view() const
{
    return d_light_proj * d_light_view;
}

const texture& shadow_map::get_texture() const
{
    return d_shadow_map.depth_texture();
}

void shadow_map::begin_frame(const glm::vec3& position, const glm::vec3& sun_dir)
{
    assert(!d_frame_data);
    d_frame_data = shadow_map_frame{};
    d_light_view = glm::lookAt(position - sun_dir, position, {0.0, 1.0, 0.0});
    d_shader.bind();
    d_shader.load("u_view_matrix", d_light_view);
    d_shader.unbind();
}

void shadow_map::end_frame()
{
    assert(d_frame_data);
    spkt::render_context rc;
    rc.depth_testing(true);

    // Reduces "peter panning", also stops "flat" objects that dont have a back from
    // casting shadows.
    rc.set_face_cull(GL_FRONT);

    d_shader.bind();
    d_shadow_map.bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    for (const auto& [key, data] : d_frame_data->commands) {
        d_instance_buffer.set_data(data);
        spkt::draw(d_asset_manager->get<static_mesh>(key), &d_instance_buffer);
    }

    d_shadow_map.unbind();
    d_shader.unbind();

    d_frame_data = std::nullopt;
}

void shadow_map::add_mesh(
    const std::string& mesh,
    const glm::vec3& position,
    const glm::quat& orientation,
    const glm::vec3& scale)
{
    assert(d_frame_data);
    d_frame_data->commands[mesh].push_back({position, orientation, scale});
}

}