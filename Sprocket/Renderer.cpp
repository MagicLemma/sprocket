#include "Renderer.h"
#include <glad/glad.h>

namespace Sprocket {

void Renderer::render(const TexturedModel& texturedModel)
{
    RawModel model = texturedModel.rawModel();

    
    model.bind();
    glDrawElements(GL_TRIANGLES, model.vertexCount(), GL_UNSIGNED_INT, nullptr);
    model.unbind();
}

}