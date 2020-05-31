#include "StreamBuffer.h"

#include <glad/glad.h>

#include <cstdlib>

namespace Sprocket {

StreamBuffer::StreamBuffer()
    : d_vao(std::make_shared<VAO>())
{
    glBindVertexArray(d_vao->Value());

    d_vertexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->Value());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    d_indexBuffer = std::make_shared<VBO>();
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
}

void StreamBuffer::Unbind() const
{
    glBindVertexArray(0);
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

}