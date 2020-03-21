#include "DisplayRenderer.h"
#include "Maths.h"
#include "Log.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Sprocket {

DisplayRenderer::DisplayRenderer(Window* window)
    : d_window(window)
    , d_quadShader("Resources/Shaders/DisplayQuad.vert",
                   "Resources/Shaders/DisplayQuad.frag")
    , d_imageShader("Resources/Shaders/DisplayImage.vert",
                    "Resources/Shaders/DisplayImage.frag")
    , d_quad({{{1.0f, 1.0f}, {1.0f, 1.0f}},
              {{1.0f, 0.0f}, {1.0f, 0.0f}},
              {{0.0f, 1.0f}, {0.0f, 1.0f}},
              {{0.0f, 0.0f}, {0.0f, 0.0f}}})
{
}

void DisplayRenderer::draw(const Quad& quad) const
{
    handleRenderOptions({false, false, false});

    d_quadShader.bind();
    d_quad.bind();

    Maths::mat4 projection = Maths::ortho(0, (float)d_window->width(),
                                          (float)d_window->height(), 0);

    Maths::mat4 transform = Maths::transform(quad.topLeftV3(), {0.0, 0.0, 0.0}, {quad.width(), quad.height(), 0.0});

    d_quadShader.loadUniform("projection", projection);
    d_quadShader.loadUniform("transform", transform);
    d_quadShader.loadUniform("colour", quad.colour());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    d_quad.unbind();
    d_quadShader.unbind();   
}

void DisplayRenderer::draw(const UiComponent& component) const
{
    handleRenderOptions({false, false, false});

    d_quadShader.bind();
    d_quad.bind();

    Maths::mat4 projection = Maths::ortho(0, (float)d_window->width(),
                                          (float)d_window->height(), 0);
    d_quadShader.loadUniform("projection", projection);

    // Background quad
    for (const auto& quad : component.quads()) {
        Maths::mat4 transform = Maths::transform(quad.topLeftV3(), {0.0, 0.0, 0.0}, {quad.width(), quad.height(), 0.0});
        d_quadShader.loadUniform("transform", transform);
        d_quadShader.loadUniform("colour", quad.colour());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    d_quad.unbind();
    d_quadShader.unbind();
}

void DisplayRenderer::draw(const Image& image) const
{
    handleRenderOptions({false, false, false});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto texture = image.texture();
    auto quad = image.quad();

    d_imageShader.bind();
    d_quad.bind();

    float width = (float)d_window->width();
    float height = (float)d_window->height();

    Maths::mat4 projection = Maths::ortho(0, width, height, 0);
    d_imageShader.loadUniform("projection", projection);

    Maths::mat4 transform = Maths::transform(image.quad().topLeftV3(),
                                            {0.0, 0.0, 0.0},
                                            {quad.width(), quad.height(), 1.0});

    d_imageShader.loadUniform("transform", transform);
    d_imageShader.loadUniform("opacity", image.opacity());
    
    texture.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    texture.unbind();

    d_quad.unbind();
    d_imageShader.unbind();

    glDisable(GL_BLEND);
}

}