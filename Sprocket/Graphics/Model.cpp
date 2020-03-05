#include "Graphics/Model.h"

#include <glad/glad.h>

namespace Sprocket {

Model::Model(unsigned int vaoId, int vertexCount, unsigned int numAttrs)
    : d_vaoId(vaoId)
    , d_vertexCount(vertexCount)
    , d_numAttrs(numAttrs)
{
}

void Model::bind() const
{
    glBindVertexArray(d_vaoId);
    for (unsigned int i = 0; i != d_numAttrs; ++i) {
        glEnableVertexAttribArray(i);
    }
}

void Model::unbind() const
{
    for (unsigned int i = 0; i != d_numAttrs; ++i) {
        glDisableVertexAttribArray(i);
    }
    glBindVertexArray(0);
}

}