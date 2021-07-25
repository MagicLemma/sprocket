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

void model_instance::set_buffer_attributes()
{
    for (int index : std::views::iota(5, 8)) {
        glEnableVertexAttribArray(index);
        glVertexAttribDivisor(index, 1);
    } 

    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(model_instance), (void*)offsetof(model_instance, position));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(model_instance), (void*)offsetof(model_instance, orientation));
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(model_instance), (void*)offsetof(model_instance, scale));
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

void unbind_vbo(std::uint32_t vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void set_data(std::uint32_t vbo, std::size_t size, const void* data, buffer_usage usage)
{
    glNamedBufferData(vbo, size, data, get_usage(usage));
}

}

}