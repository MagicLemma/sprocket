#pragma once
#include <Sprocket/Graphics/buffer_element_types.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstddef>
#include <functional>
#include <span>

namespace spkt {

enum class buffer_usage
{
    STATIC,
    DYNAMIC,
    STREAM
};

namespace detail {

std::uint32_t new_vbo();
void delete_vbo(std::uint32_t vbo);
void bind_index_buffer(std::uint32_t vbo);
void set_data(std::uint32_t vbo, std::size_t size, const void* data, buffer_usage usage);

}

template <typename T, buffer_usage Usage, void(*BindFunc)(std::uint32_t)> 
class basic_buffer
{
    std::uint32_t d_vbo;
    std::size_t   d_size;

    basic_buffer(const basic_buffer&) = delete;
    basic_buffer& operator=(const basic_buffer&) = delete;

public:
    basic_buffer(std::span<const T> data) : basic_buffer() { set_data(data); }
    basic_buffer() : d_vbo(detail::new_vbo()), d_size(0) {}
    ~basic_buffer() { detail::delete_vbo(d_vbo); }

    void bind() const { BindFunc(d_vbo); }

    void set_data(std::span<const T> data)
    {
        d_size = data.size();
        detail::set_data(d_vbo, data.size() * sizeof(T), data.data(), Usage);
    }

    std::size_t size() const { return d_size; }
};

template <spkt::buffer_element T, buffer_usage Usage = buffer_usage::STATIC>
using vertex_buffer = basic_buffer<T, Usage, T::set_buffer_attributes>;

template <std::unsigned_integral T, buffer_usage Usage = buffer_usage::STATIC>
using index_buffer = basic_buffer<T, Usage, detail::bind_index_buffer>;

}