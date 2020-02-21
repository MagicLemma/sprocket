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
    glm::mat4 transform = Maths::createTransformationMatrix(
        entity.position(),
        entity.rotation(),
        entity.scale());

    glm::mat4 view = Maths::createViewMatrix(
        camera.position(),
        camera.pitch(),
        camera.yaw(),
        camera.roll()
    );

    entity.bind();
    shader.bind();
    shader.loadMatrix4f("transformMatrix", transform);
    shader.loadMatrix4f("viewMatrix", view);

    glDrawElements(GL_TRIANGLES, entity.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    
    shader.unbind();
    entity.unbind();
}

}