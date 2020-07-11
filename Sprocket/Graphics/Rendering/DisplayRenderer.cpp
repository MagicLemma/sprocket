#include "DisplayRenderer.h"
#include "Maths.h"
#include "Log.h"
#include "RenderContext.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Sprocket {
namespace {

Vertex2DBuffer GetQuad()
{
    return Vertex2DBuffer{
        {
            Maths::vec2{1.0f, 1.0f},
            Maths::vec2{1.0f, 1.0f},
            Maths::vec3{-1.0f, 1.0f, 1.0f}
        }, {
             Maths::vec2{1.0f, 0.0f},
             Maths::vec2{1.0f, 0.0f},
             Maths::vec3{-1.0f, 1.0f, 1.0f}
        }, {
             Maths::vec2{0.0f, 1.0f},
             Maths::vec2{0.0f, 1.0f},
             Maths::vec3{-1.0f, 1.0f, 1.0f}
        }, {
             Maths::vec2{0.0f, 0.0f},
             Maths::vec2{0.0f, 0.0f},
             Maths::vec3{-1.0f, 1.0f, 1.0f}
        }
    };
}

}

DisplayRenderer::DisplayRenderer(Window* window)
    : d_window(window)
    , d_colourShader("Resources/Shaders/DisplayQuad.vert",
                     "Resources/Shaders/DisplayQuad.frag")
    , d_quad(GetQuad())
{
}

void DisplayRenderer::OnUpdate() const
{
    float width = (float)d_window->Width();
    float height = (float)d_window->Height();
    Maths::mat4 projection = Maths::Ortho(0, width, height, 0);

    d_colourShader.Bind();
    d_colourShader.LoadUniform("projection", projection);
}

void DisplayRenderer::Draw(const Quad& quad) const
{
    Draw(quad, d_quad);
}

void DisplayRenderer::Draw(const Quad& quad, const Model2D& model) const
{
    RenderContext rc;
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto position3D = Maths::vec3{quad.position.x, quad.position.y, 0.0f};
    auto transform = Maths::Transform(position3D, Maths::identity);
    transform = Maths::Scale(transform, {quad.width, quad.height, 0.0});

    // Find the appropriate shader and bind the colour/texture.
    d_colourShader.Bind();

    d_colourShader.LoadUniform("colour", quad.colour);
    d_colourShader.LoadUniform("transform", transform);
    d_colourShader.LoadUniform("opacity", quad.opacity);
    d_colourShader.LoadUniform("roundness", quad.roundness);
    d_colourShader.LoadUniform("greyscale", quad.greyscale ? 1.0f : 0.0f);

    quad.texture.Bind();
    model.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    model.Unbind();
    quad.texture.Unbind();

    d_colourShader.Unbind();
    glDisable(GL_BLEND);
}

}