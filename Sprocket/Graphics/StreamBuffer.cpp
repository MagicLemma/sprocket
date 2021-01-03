#include "StreamBuffer.h"

#include <glad/glad.h>

#include <cstdlib>

namespace Sprocket {

StreamBuffer::StreamBuffer()
    : d_vao(std::make_unique<VAO>())
    , d_vertexBuffer(std::make_unique<VBO>())
    , d_indexBuffer(std::make_unique<VBO>())
{
    // Set the index buffer pointer in the VAO.
    glBindVertexArray(d_vao->Value());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->Value());
    glBindVertexArray(0);
}

StreamBuffer::~StreamBuffer()
{
    Unbind();
}

void StreamBuffer::Bind() const
{
    glBindVertexArray(d_vao->Value());
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
}

void StreamBuffer::Unbind() const
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StreamBuffer::SetBufferLayout(const BufferLayout& layout) const
{
    if (layout.Validate()) {
        Bind();
        layout.SetAttributes();
        Unbind();
    }
    else {
        SPKT_LOG_ERROR("Buffer layout is not complete!");
    }
}

void StreamBuffer::SetVertexData(std::size_t size, const void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
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