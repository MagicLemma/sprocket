#include "Model3D.h"

#include <glad/glad.h>

namespace Sprocket {

Model3D::Model3D(unsigned int vaoId, int vertexCount)
    : d_vaoId(vaoId)
    , d_vertexCount(vertexCount)
{
}

Model3D::Model3D(const Model3D& model)
    : d_vaoId(model.d_vaoId)
    , d_vertexCount(model.d_vertexCount)
{
}

void Model3D::bind() const
{
    glBindVertexArray(d_vaoId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void Model3D::unbind() const
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}

}