#include "Renderer.h"
#include "Maths.h"
#include "Log.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Sprocket {

void Renderer::render(const Entity& entity,
                      const Camera& camera,
                      const Shader& shader)
{
    entity.model().bind();
    entity.texture().bind();

    glm::mat4 transform = Maths::createTransformationMatrix(
        entity.position(),
        entity.rotation(),
        entity.scale());

    glm::mat4 projection = Maths::createProjectionMatrix(
        camera.aspectRatio(),
        camera.fov(),
        camera.nearPlane(),
        camera.farPlane());

    shader.bind();
    shader.loadMatrix4f("transformMatrix", transform);
    shader.loadMatrix4f("projectionMatrix", projection);

    glDrawElements(GL_TRIANGLES, entity.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    
    shader.unbind();
    entity.texture().unbind();
    entity.model().unbind();
}

}