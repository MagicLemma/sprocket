#include "Renderer.h"
#include <glad/glad.h>

namespace Sprocket {

void Renderer::render(const TexturedModel& texturedModel)
{
    RawModel model = texturedModel.rawModel();
    Texture tex = texturedModel.texture();

    model.bind();
    tex.bind();
    glDrawElements(GL_TRIANGLES, model.vertexCount(), GL_UNSIGNED_INT, nullptr);
    tex.unbind();
    model.unbind();
}

}