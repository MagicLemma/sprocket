#include "Renderer.h"
#include "Maths.h"
#include "Log.h"

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Sprocket {

Renderer::Renderer()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Renderer::render(const Entity& entity,
                      const std::vector<Light>& lights,
                      const Camera& camera,
                      const Shader& shader)
{   
    shader.bind();
    shader.loadEntity(entity);
    shader.loadLights(lights);
    shader.loadCamera(camera);

    entity.bind();
    glDrawElements(GL_TRIANGLES, entity.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    entity.unbind();

    shader.unbind();
}

}