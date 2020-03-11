#include "Graphics/Skyboxes/SkyboxRenderer.h"

#include <glad/glad.h>

namespace Sprocket {
    
SkyboxRenderer::SkyboxRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
{
}

void SkyboxRenderer::draw(const Skybox& skybox,
                          const Camera& camera,
                          const Lens& lens,
                          const RenderOptions& options)
{
    handleRenderOptions(options);
    glDepthMask(true);

    d_shader.bind();
    Maths::mat4 projection = lens.projection();

    Maths::mat4 view = Maths::createViewMatrix(
        camera.position(),
        camera.pitch(),
        camera.yaw(),
        camera.roll()
    );
    view = Maths::mat4(Maths::mat3(view));
    
    d_shader.loadUniform("projectionMatrix", projection);
    d_shader.loadUniform("viewMatrix", view);

    skybox.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36/*skybox.model()->vertexCount()*/);
    skybox.unbind();

    d_shader.unbind();
}

}