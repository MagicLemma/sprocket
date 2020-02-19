#include "Renderer.h"
#include <glad/glad.h>

namespace Sprocket {

void Renderer::render(const Model& model, const Texture& texture)
{
    model.bind();
    texture.bind();
    glDrawElements(GL_TRIANGLES, model.vertexCount(), GL_UNSIGNED_INT, nullptr);
    texture.unbind();
    model.unbind();
}

}