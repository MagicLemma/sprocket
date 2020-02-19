#include "Renderer.h"
#include <glad/glad.h>

namespace Sprocket {

void Renderer::render(const Entity& entity, const Shader& shader)
{
    entity.model().bind();
    entity.texture().bind();
    shader.bind();
    glDrawElements(GL_TRIANGLES, entity.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    shader.unbind();
    entity.texture().unbind();
    entity.model().unbind();
}

}