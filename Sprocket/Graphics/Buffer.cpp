#include "Buffer.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

int GetUsage(BufferUsage usage)
{
    switch (usage) {
        case BufferUsage::STATIC: return GL_STATIC_DRAW;
        case BufferUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
    }
}

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

}