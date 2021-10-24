#pragma once
#include <sprocket/Graphics/asset_manager.h>
#include <sprocket/Graphics/buffer.h>
#include <sprocket/Graphics/frame_buffer.h>
#include <sprocket/Graphics/open_gl.h>
#include <sprocket/Graphics/shader.h>
#include <sprocket/Graphics/texture.h>

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