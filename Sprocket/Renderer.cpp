#include "Renderer.h"
#include <glad/glad.h>

namespace Sprocket {

void Renderer::render(const RawModel& model)
{
    glBindVertexArray(model.vaoId());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawElements(GL_TRIANGLES, model.vertexCount(), GL_UNSIGNED_INT, nullptr);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

}