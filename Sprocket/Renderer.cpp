#include "Renderer.h"
#include <glad/glad.h>

namespace Sprocket {

void Renderer::render(const RawModel& model)
{
    glBindVertexArray(model.vaoId());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, model.vertexCount());
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

}