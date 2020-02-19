#include "Renderer.h"
#include <glad/glad.h>

namespace Sprocket {

void Renderer::render(const RawModel& model)
{
    model.bind();
    glDrawElements(GL_TRIANGLES, model.vertexCount(), GL_UNSIGNED_INT, nullptr);
    model.unbind();
}

}