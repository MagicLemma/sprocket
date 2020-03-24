#include "DisplayRenderer.h"
#include "Maths.h"
#include "Log.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Sprocket {

DisplayRenderer::DisplayRenderer(Window* window)
    : d_window(window)
    , d_colourShader("Resources/Shaders/DisplayColoured.vert",
                     "Resources/Shaders/DisplayColoured.frag")
    , d_textureShader("Resources/Shaders/DisplayTextured.vert",
                      "Resources/Shaders/DisplayTextured.frag")
    , d_quad({{{1.0f, 1.0f}, {1.0f, 1.0f}},
              {{1.0f, 0.0f}, {1.0f, 0.0f}},
              {{0.0f, 1.0f}, {0.0f, 1.0f}},
              {{0.0f, 0.0f}, {0.0f, 0.0f}}})
{
}

void DisplayRenderer::update() const
{
    float width = (float)d_window->width();
    float height = (float)d_window->height();
    Maths::mat4 projection = Maths::ortho(0, width, height, 0);

    d_colourShader.bind();
    d_colourShader.loadUniform("projection", projection);

    d_textureShader.bind();
    d_textureShader.loadUniform("projection", projection);
    d_textureShader.unbind();
}

void DisplayRenderer::draw(const Widget& widget) const
{
    for (const auto& quad : widget.quads()) {
        draw(widget, quad);
    }

    for (const auto& child : widget.children()) {
        draw(*child.get());
    }
}

void DisplayRenderer::draw(const Widget& widget, const VisualQuad& quad) const
{
    handleRenderOptions({false, false, false});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto position3D = Maths::vec3{quad.body.position.x, quad.body.position.y, 0.0f};
    auto transform = Maths::transform(position3D, {0.0, 0.0, 0.0}, {quad.body.width, quad.body.height, 0.0});

    // Find the appropriate shader and bind the colour/texture.
    const Shader* shader;
    if (std::holds_alternative<Colour>(quad.skin)) {
        shader = &d_colourShader;
        shader->bind();
        shader->loadUniform("colour", std::get<Colour>(quad.skin));
    }
    else if (std::holds_alternative<Texture>(quad.skin)) {
        shader = &d_textureShader;
        shader->bind();
        std::get<Texture>(quad.skin).bind();
    }
    else {
        SPKT_LOG_ERROR("Quad has unknown skin!");
        return;
    }

    shader->loadUniform("transform", transform);
    shader->loadUniform("opacity", quad.opacity);
    shader->loadUniform("roundness", quad.roundness);
    shader->loadUniform("greyscale", widget.active() ? 0.0f : 1.0f);

    d_quad.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    d_quad.unbind();

    // If it was a textured Quad, unbind the texture.
    if (std::holds_alternative<Texture>(quad.skin)) {
        std::get<Texture>(quad.skin).unbind();
    }

    shader->unbind();
    glDisable(GL_BLEND);
}

void DisplayRenderer::draw(const VisualQuad& quad) const
{
    handleRenderOptions({false, false, false});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto position3D = Maths::vec3{quad.body.position.x, quad.body.position.y, 0.0f};
    auto transform = Maths::transform(position3D, {0.0, 0.0, 0.0}, {quad.body.width, quad.body.height, 0.0});

    // Find the appropriate shader and bind the colour/texture.
    const Shader* shader;
    if (std::holds_alternative<Colour>(quad.skin)) {
        shader = &d_colourShader;
        shader->bind();
        shader->loadUniform("colour", std::get<Colour>(quad.skin));
    }
    else if (std::holds_alternative<Texture>(quad.skin)) {
        shader = &d_textureShader;
        shader->bind();
        std::get<Texture>(quad.skin).bind();
    }
    else {
        SPKT_LOG_ERROR("Quad has unknown skin!");
        return;
    }

    shader->loadUniform("transform", transform);
    shader->loadUniform("opacity", quad.opacity);
    shader->loadUniform("roundness", quad.roundness);
    shader->loadUniform("greyscale", 0.0f);

    d_quad.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    d_quad.unbind();

    // If it was a textured Quad, unbind the texture.
    if (std::holds_alternative<Texture>(quad.skin)) {
        std::get<Texture>(quad.skin).unbind();
    }

    shader->unbind();
    glDisable(GL_BLEND);
}

}