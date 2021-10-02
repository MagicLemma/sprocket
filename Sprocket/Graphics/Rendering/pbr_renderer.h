#pragma once
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/shadow_map.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Utility/Hashing.h>

#include <memory>
#include <optional>
#include <functional>

namespace spkt {

// PBR Texture Slots
static constexpr int ALBEDO_SLOT = 0;
static constexpr int NORMAL_SLOT = 1;
static constexpr int METALLIC_SLOT = 2;
static constexpr int ROUGHNESS_SLOT = 3;

// Shadow Map Texture Slot
static constexpr int SHADOW_MAP_SLOT = 4;

// Animation Data
static constexpr int MAX_BONES = 50;

// Light Data
static constexpr int MAX_NUM_LIGHTS = 50;

struct frame_data
{
    std::unordered_map<
        std::pair<std::string, std::string>,
        std::vector<spkt::model_instance>,
        spkt::hash_pair
    > static_mesh_draw_commands;

    std::array<glm::vec3, spkt::MAX_NUM_LIGHTS> light_positions;
    std::array<glm::vec3, spkt::MAX_NUM_LIGHTS> light_colours;
    std::array<float, spkt::MAX_NUM_LIGHTS>     light_brightnesses;
    std::size_t next_light_index;
};

class pbr_renderer
{
    spkt::asset_manager* d_assetManager;

    spkt::shader d_staticShader;
    spkt::shader d_animatedShader;

    void for_each_shader(const std::function<void(spkt::shader& shader)>& callback);
    
    spkt::vertex_buffer<spkt::model_instance> d_instanceBuffer;

    std::optional<frame_data> d_frame_data;

    pbr_renderer(const pbr_renderer&) = delete;
    pbr_renderer& operator=(const pbr_renderer&) = delete;

public:
    pbr_renderer(asset_manager* assetManager);

    void begin_frame(const glm::mat4& proj, const glm::mat4& view);
    void end_frame();

    void set_ambience(const glm::vec3& colour, const float brightness);
    void set_sunlight(const glm::vec3& colour, const glm::vec3& direction, const float brightness);
    void add_light(const glm::vec3& position, const glm::vec3& colour, const float brightness);

    void draw_static_mesh(
        const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale,
        const std::string& mesh, const std::string& material
    );

    void draw_animated_mesh(
        const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale,
        const std::string& mesh, const std::string& material,
        const std::string& animation_name, float animation_time
    );

    void draw_particles(
        std::span<const spkt::model_instance> particles
    );

    void enable_shadows(const shadow_map& shadowMap);

    spkt::shader& static_shader() { return d_staticShader; }
    spkt::shader& animated_shader() { return d_animatedShader; }
};

}