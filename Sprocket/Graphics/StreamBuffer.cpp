#include "StreamBuffer.h"

#include <Sprocket/Utility/Log.h>

#include <glad/glad.h>

#include <cstdlib>

namespace spkt {

StreamBuffer::StreamBuffer()
    : d_vao()
    , d_vertexBuffer()
    , d_indexBuffer()
{
}

StreamBuffer::~StreamBuffer()
{
    Unbind();
}

void StreamBuffer::SetVertexData(std::size_t size, const void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void StreamBuffer::SetIndexData(std::size_t size, const void* data)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void StreamBuffer::Draw(int count) const
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

}