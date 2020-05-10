#include "Model2D.h"

#include <glad/glad.h>

namespace Sprocket {

Model2D::Model2D(const Vertex2DBuffer& vertices)
    : d_vao(std::make_shared<VAO>())
    , d_vertexBuffer(std::make_shared<VBO>())
    , d_vertexCount(vertices.size())
{
    glBindVertexArray(d_vao->value());
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->value());

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set Vertex Attributes in the VAO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, texture));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, colour));

    glBindVertexArray(0);
}

void Model2D::bind() const
{
    glBindVertexArray(d_vao->value());
}

void Model2D::unbind() const
{
    glBindVertexArray(0);
}

}