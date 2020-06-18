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
                          const Entity& camera)
{
    Maths::vec3 position = camera.Position();
    Maths::quat orientation = camera.Orientation();
    auto c = camera.Get<CameraComponent>();
    orientation *= Maths::Rotate({1, 0, 0}, c.pitch);

    Draw(skybox,
         Maths::Inverse(Maths::Transform(position, orientation)),
         camera.Get<CameraComponent>().lens->Projection());
}

void SkyboxRenderer::Draw(const Skybox& skybox,
                          const Maths::mat4& view,
                          const Maths::mat4& proj)
{
    RenderContext rc;
    glDisable(GL_CULL_FACE);
    glDepthMask(true);

    d_shader.Bind();

    Maths::mat4 view2 = Maths::mat4(Maths::mat3(view));
    
    d_shader.LoadUniform("projectionMatrix", proj);
    d_shader.LoadUniform("viewMatrix", view2);

    skybox.model.Bind();
    skybox.texture.Bind();
    glDrawElements(GL_TRIANGLES, skybox.model.VertexCount(), GL_UNSIGNED_INT, (const void*)0);
    skybox.texture.Unbind();
    skybox.model.Unbind();

    d_shader.Unbind();
}

void SkyboxRenderer::Draw(const Skybox& skybox,
                          const Camera& camera,
                          const Lens& lens)
{
    Draw(skybox, camera.View(), lens.Projection());
}

}