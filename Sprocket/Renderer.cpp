#include "Renderer.h"
#include "Maths.h"
#include "Log.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Sprocket {

void Renderer::render(const Entity& entity, const Shader& shader)
{
    entity.model().bind();
    entity.texture().bind();

    glm::mat4 matrix = Maths::createTransformationMatrix(
        entity.position(),
        entity.rotation(),
        entity.scale());

    shader.bind();
    shader.loadMatrix4f("transformMatrix", matrix);

    glDrawElements(GL_TRIANGLES, entity.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    shader.unbind();
    entity.texture().unbind();
    entity.model().unbind();
}

}