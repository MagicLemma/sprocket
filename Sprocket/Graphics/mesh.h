#pragma once
#include <Sprocket/Graphics/Animation.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/buffer_element_types.h>

#include <glm/glm.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace spkt {

struct static_mesh_data
{
    std::vector<static_vertex> vertices;
    std::vector<std::uint32_t> indices;

    static static_mesh_data load(const std::string& file);
};

struct animated_mesh_data
{
    std::vector<animated_vertex> vertices;
    std::vector<std::uint32_t>   indices;
    Skeleton skeleton;

    static animated_mesh_data load(const std::string& file);
};

class static_mesh
{
    spkt::vertex_buffer<spkt::static_vertex> d_vertices;
    spkt::index_buffer<std::uint32_t>        d_indices;

    static_mesh(const static_mesh&) = delete;
    static_mesh& operator=(const static_mesh&) = delete;

public:
    static_mesh(const static_mesh_data& data = {});

    static std::unique_ptr<static_mesh> from_file(const std::string& file);

    std::size_t vertex_count() const { return d_indices.size(); }

    void bind() const;
};

using static_mesh_ptr = std::unique_ptr<static_mesh>;

class animated_mesh
{
    spkt::vertex_buffer<spkt::animated_vertex> d_vertices;
    spkt::index_buffer<std::uint32_t>          d_indices;
    spkt::Skeleton                             d_skeleton;

    animated_mesh(const animated_mesh&) = delete;
    animated_mesh& operator=(const animated_mesh&) = delete;

public:
    animated_mesh(const animated_mesh_data& data = {});

    static std::unique_ptr<animated_mesh> from_file(const std::string& file);

    std::size_t vertex_count() const { return d_indices.size(); }

    void bind() const;

    // Returns the transforms to be uploaded to the shader. The transform
    // at position i corresponds to the bone with ID i.
    std::vector<glm::mat4> get_pose(const std::string& name, float time) const;

    // Returns a list of names of all possible animations in this mesh.
    std::vector<std::string> get_animation_names() const;
};

using animated_mesh_ptr = std::unique_ptr<animated_mesh>;

}