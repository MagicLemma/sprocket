#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstddef>
#include <span>

namespace spkt {

struct model_instance
{
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;

    static void set_buffer_attributes();
};

template <typename T>
concept buffer_element = requires
{
    { T::set_buffer_attributes() } -> std::same_as<void>;
};

enum class buffer_usage
{
    STATIC,
    DYNAMIC
};

namespace detail {

std::uint32_t new_vbo();
void delete_vbo(std::uint32_t vbo);
void bind_vbo(std::uint32_t vbo);
void unbind_vbo(std::uint32_t vbo);
void set_data(std::uint32_t vbo, std::size_t size, const void* data, buffer_usage usage);

}

template <buffer_element T, buffer_usage Usage = buffer_usage::STATIC>
class buffer
{
    std::uint32_t d_vbo;
    std::size_t   d_size;

    buffer(const buffer&) = delete;
    buffer& operator=(const buffer&) = delete;

public:
    buffer() : d_vbo(detail::new_vbo()), d_size(0) {}
    ~buffer() { detail::delete_vbo(d_vbo); }

    void bind() const
    {
        detail::bind_vbo(d_vbo);
        T::set_buffer_attributes();
        detail::unbind_vbo(d_vbo);
    }

    void set_data(std::span<const T> data)
    {
        d_size = data.size();
        detail::set_data(d_vbo, data.size_bytes(), data.data(), Usage);
    }

    std::size_t size() const { return d_size; }
};

}