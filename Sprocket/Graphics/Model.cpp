#include "Graphics/Model.h"

#include <glad/glad.h>

namespace Sprocket {

Model::Model(unsigned int vaoId, int vertexCount, ModelType type)
    : d_vaoId(vaoId)
    , d_vertexCount(vertexCount)
    , d_type(type)
{
}

void Model::bind() const
{
    glBindVertexArray(d_vaoId);
    if (d_type == ModelType::ENTITY) {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
    else if (d_type == ModelType::FLAT) {
        glEnableVertexAttribArray(0);
    }
}

void Model::unbind() const
{
    if (d_type == ModelType::ENTITY) {
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
    else if (d_type == ModelType::FLAT) {
        glDisableVertexAttribArray(0);
    }
    glBindVertexArray(0);
}

}