#include "Graphics/Modelling/ModelSkybox.h"

namespace Sprocket {

ModelSkybox::ModelSkybox(unsigned int vaoId, int vertexCount)
    : d_vaoId(vaoId)
    , d_vertexCount(vertexCount)
{
}

void ModelSkybox::bind() const
{
    glBindVertexArray(d_vaoId);
    glEnableVertexAttribArray(0);
}

void ModelSkybox::unbind() const
{
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

}