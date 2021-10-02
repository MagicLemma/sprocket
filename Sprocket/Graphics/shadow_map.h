#pragma once
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/frame_buffer.h>
#include <Sprocket/Graphics/open_gl.h>
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/texture.h>
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>
#include <optional>

namespace spkt {

struct shadow_map_frame
{
    std::unordered_map<std::string, std::vector<model_instance>> commands;
};

class shadow_map
{
    spkt::asset_manager*                      d_asset_manager;
    spkt::shader                              d_shader;
    spkt::frame_buffer                        d_shadow_map;
    spkt::vertex_buffer<spkt::model_instance> d_instance_buffer;

    std::optional<shadow_map_frame> d_frame_data;

    glm::mat4 d_light_view;
    glm::mat4 d_light_proj;

public:
    shadow_map(spkt::asset_manager* assetManager);

    void draw(spkt::registry& registry, const glm::vec3& sun_dir, const glm::vec3& centre);

    void begin_frame(const glm::vec3& position, const glm::vec3& sun_dir);
    void end_frame();

    void add_mesh(
        const std::string& mesh,
        const glm::vec3& position,
        const glm::quat& orientation,
        const glm::vec3& scale
    );

    glm::mat4 get_light_proj_view() const;
    const texture& get_texture() const;
};

}