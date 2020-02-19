#include "Renderer.h"
#include <glad/glad.h>

namespace Sprocket {

void Renderer::render(const RawModel& rawModel, const Texture& texture)
{
    rawModel.bind();
    texture.bind();
    glDrawElements(GL_TRIANGLES, rawModel.vertexCount(), GL_UNSIGNED_INT, nullptr);
    texture.unbind();
    rawModel.unbind();
}

}