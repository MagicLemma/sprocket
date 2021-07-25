#include "Buffer.h"

#include <glad/glad.h>

#include <ranges>

namespace spkt {
namespace {

int GetUsage(BufferUsage usage)
{
    switch (usage) {
        case BufferUsage::STATIC: return GL_STATIC_DRAW;
        case BufferUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
    }
}

}

void InstanceData::set_buffer_attributes()
{
    for (int index : std::views::iota(5, 8)) {
        glEnableVertexAttribArray(index);
        glVertexAttribDivisor(index, 1);
    } 

    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, position));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, orientation));
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, scale));
}

Buffer::Buffer(
    const BufferLayout& layout,
    const BufferUsage& usage
)
    : d_layout(layout)
    , d_usage(usage)
    , d_instanceCount(0)
    , d_instanceSize(0)
{
    glCreateBuffers(1, &d_vbo);
    assert(d_layout.Validate());
}

void Buffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vbo);
    d_layout.SetAttributes();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::SetData(
    std::size_t instanceCount,
    std::size_t instanceSize,
    const void* data
)
{
    d_instanceCount = instanceCount;
    d_instanceSize = instanceSize;
    glNamedBufferData(d_vbo, instanceCount * instanceSize, data, GetUsage(d_usage));
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

void set_data(
    std::uint32_t vbo,
    std::size_t size,
    const void* data,
    BufferUsage usage)
{
    glNamedBufferData(vbo, size, data, GetUsage(usage));
}

}

}