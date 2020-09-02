#include "InstanceBuffer.h"

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

InstanceBuffer::InstanceBuffer(
    const BufferLayout& layout,
    const BufferUsage& usage
)
    : d_buffer(std::make_shared<VBO>())
    , d_layout(layout)
    , d_usage(usage)
    , d_instanceCount(0)
    , d_instanceSize(0)
{
    glBindBuffer(GL_ARRAY_BUFFER, d_buffer->Value());

    // This will resize next time we set the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * 1000, nullptr, GetUsage(d_usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    assert(d_layout.Validate());
}

void InstanceBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, d_buffer->Value());
    d_layout.SetAttributes();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceBuffer::SetData(
    std::size_t instanceCount,
    std::size_t instanceSize,
    const void* data
)
{
    d_instanceCount = instanceCount;
    d_instanceSize = instanceSize;
    glBindBuffer(GL_ARRAY_BUFFER, d_buffer->Value());
    glBufferData(GL_ARRAY_BUFFER, instanceCount * instanceSize, data, GetUsage(d_usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}