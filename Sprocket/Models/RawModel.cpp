#include "RawModel.h"

#include <glad/glad.h>

namespace Sprocket {

RawModel::RawModel(unsigned int vaoId, int vertexCount)
    : d_vaoId(vaoId)
    , d_vertexCount(vertexCount)
{
}

void RawModel::bind() const
{
    glBindVertexArray(d_vaoId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void RawModel::unbind() const
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

}