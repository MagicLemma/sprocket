#include "Renderer.h"
#include "Maths.h"
#include "Log.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Sprocket {

Renderer::Renderer()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Renderer::render(const Entity& entity,
                      const Light& light,
                      const Camera& camera,
                      const Shader& shader)
{
    shader.bind();
    shader.loadEntity(entity);
    shader.loadLight(light);
    shader.loadCamera(camera);

    entity.bind();
    glDrawElements(GL_TRIANGLES, entity.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    entity.unbind();

    shader.unbind();
}

}