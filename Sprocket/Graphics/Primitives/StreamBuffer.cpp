#include "StreamBuffer.h"

#include <glad/glad.h>

#include <cstdlib>

namespace Sprocket {

StreamBuffer::StreamBuffer()
    : d_vao(std::make_shared<VAO>())
{
    glBindVertexArray(d_vao->value());

    d_vertexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->value());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    d_indexBuffer = std::make_shared<VBO>();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->value());

    glBindVertexArray(0);
}

StreamBuffer::~StreamBuffer()
{
    unbind();
}

void StreamBuffer::bind() const
{
    glBindVertexArray(d_vao->value());
}

void StreamBuffer::unbind() const
{
    glBindVertexArray(0);
}

void StreamBuffer::setVertexData(std::size_t size, const void* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->value());
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void StreamBuffer::setIndexData(std::size_t size, const void* data)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

}