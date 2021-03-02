#include "StreamBuffer.h"
#include "Log.h"

#include <glad/glad.h>

#include <cstdlib>

namespace Sprocket {

StreamBuffer::StreamBuffer()
    : d_vao(0)
    , d_vertexBuffer(0)
    , d_indexBuffer(0)
{
    glGenVertexArrays(1, &d_vao);
    glGenBuffers(1, &d_vertexBuffer);
    glGenBuffers(1, &d_indexBuffer);

    // Set the index buffer pointer in the vertex buffer.
    glBindVertexArray(d_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer);
    glBindVertexArray(0);
}

StreamBuffer::~StreamBuffer()
{
    Unbind();

    glDeleteBuffers(1, &d_indexBuffer);
    glDeleteBuffers(1, &d_vertexBuffer);
    glDeleteVertexArrays(1, &d_vao);
}

void StreamBuffer::Bind() const
{
    glBindVertexArray(d_vao);
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer);
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
        log::error("Buffer layout is not complete!");
    }
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