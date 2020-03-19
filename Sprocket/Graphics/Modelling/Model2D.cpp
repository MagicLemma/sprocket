#include "Graphics/Modelling/Model2D.h"
#include "Loader.h"

#include <glad/glad.h>

namespace Sprocket {

Model2D::Model2D(const Vertex2DBuffer& buffer)
    : d_vaoId(Loader::createVAO())
    , d_vertexCount(buffer.size())
{
    // Bind a Vertex Buffer
    Loader::createVBO(Loader::VBOType::VERTEX_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * buffer.size(), buffer.data(), GL_STATIC_DRAW);

    // Set Vertex Attributes in the VAO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, position));

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, texture));

    // Unbind the VAO
    glBindVertexArray(0);
}

void Model2D::bind() const
{
    glBindVertexArray(d_vaoId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Model2D::unbind() const
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

}