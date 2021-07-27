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
        case buffer_usage::STREAM: return GL_STREAM_DRAW;
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

void bind_index_buffer(std::uint32_t vbo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
}

void set_data(std::uint32_t vbo, std::size_t size, const void* data, buffer_usage usage)
{
    glNamedBufferData(vbo, size, data, get_usage(usage));
}

}

}