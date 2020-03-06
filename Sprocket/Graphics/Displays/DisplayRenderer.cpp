#include "Graphics/Displays/DisplayRenderer.h"
#include "Utility/Maths.h"

#include <glad/glad.h>

namespace Sprocket {

DisplayRenderer::DisplayRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Display.vert",
               "Resources/Shaders/Display.frag")
{

}

void DisplayRenderer::draw(
    std::shared_ptr<Model2D> model,
    const RenderOptions& options)
{
    handleRenderOptions(options);
    d_shader.bind();
    model->bind();
    glDrawArrays(GL_TRIANGLES, 0, model->vertexCount());
    model->unbind();
    d_shader.unbind();
}

}