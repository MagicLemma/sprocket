#include "SkyboxRenderer.h"
#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {
    
SkyboxRenderer::SkyboxRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Skybox.vert",
               "Resources/Shaders/Skybox.frag")
{
}

void SkyboxRenderer::Draw(const Skybox& skybox,
                          const Camera& camera,
                          const Lens& lens)
{
    RenderContext rc;
    glDisable(GL_CULL_FACE);
    glDepthMask(true);

    d_shader.Bind();
    Maths::mat4 projection = lens.Projection();

    Maths::mat4 view = camera.View();
    view = Maths::mat4(Maths::mat3(view));
    
    d_shader.LoadUniform("projectionMatrix", lens.Projection());
    d_shader.LoadUniform("viewMatrix", view);

    skybox.model.Bind();
    skybox.texture.Bind();
    glDrawElements(GL_TRIANGLES, skybox.model.VertexCount(), GL_UNSIGNED_INT, (const void*)0);
    skybox.texture.Unbind();
    skybox.model.Unbind();

    d_shader.Unbind();
}

}