#pragma once
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/frame_buffer.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/texture.h>
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>

namespace spkt {

class shadow_map
{
    spkt::asset_manager*                      d_asset_manager;
    spkt::shader                              d_shader;
    spkt::frame_buffer                        d_shadow_map;
    spkt::vertex_buffer<spkt::model_instance> d_instance_buffer;

    glm::mat4 d_light_view;
    glm::mat4 d_light_proj;

public:
    shadow_map(spkt::asset_manager* assetManager);

    void draw(spkt::registry& registry, const glm::vec3& sun_dir, const glm::vec3& centre);

    glm::mat4 get_light_proj_view() const;
    const texture& get_texture() const;
};

}