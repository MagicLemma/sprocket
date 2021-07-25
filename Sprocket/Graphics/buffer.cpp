#include "buffer.h"

#include <glad/glad.h>

#include <ranges>

namespace spkt {
namespace {

int get_usage(buffer_usage usage)
{
    switch (usage) {
        case buffer_usage::STATIC: return GL_STATIC_DRAW;
        case buffer_usage::DYNAMIC: return GL_DYNAMIC_DRAW;
    }
}

}

namespace detail {

std::uint32_t new_vbo()
{
    std::uint32_t vbo = 0;
    glCreateBuffers(1, &vbo);
    return vbo;
}

void delete_vbo(std::uint32_t vbo)
{
    glDeleteBuffers(1, &vbo);
}

void bind_vbo(std::uint32_t vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void unbind_vbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void set_data(std::uint32_t vbo, std::size_t size, const void* data, buffer_usage usage)
{
    glNamedBufferData(vbo, size, data, get_usage(usage));
}

}

index_buffer::index_buffer()
    : d_vbo(detail::new_vbo())
    , d_size(0)
{}

index_buffer::~index_buffer()
{
    detail::delete_vbo(d_vbo);
}

void index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_vbo);
}

void index_buffer::set_data(std::span<const std::uint32_t> data)
{
    d_size = data.size();
    detail::set_data(d_vbo, data.size_bytes(), data.data(), buffer_usage::STATIC);
}

}