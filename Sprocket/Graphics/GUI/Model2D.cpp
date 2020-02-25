#include "Graphics/GUI/Model2D.h"

#include <glad/glad.h>

namespace Sprocket {

Model2D::Model2D(unsigned int vaoId, int vertexCount)
    : d_vaoId(vaoId)
    , d_vertexCount(vertexCount)
{
}

void Model2D::bind() const
{
    glBindVertexArray(d_vaoId);
    glEnableVertexAttribArray(0);
}

void Model2D::unbind() const
{
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

}