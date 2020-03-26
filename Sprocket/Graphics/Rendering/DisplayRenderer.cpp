#include "DisplayRenderer.h"
#include "Maths.h"
#include "Log.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Sprocket {

DisplayRenderer::DisplayRenderer(Window* window)
    : d_window(window)
    , d_colourShader("Resources/Shaders/DisplayQuad.vert",
                     "Resources/Shaders/DisplayQuad.frag")
    , d_characterShader("Resources/Shaders/DisplayCharacter.vert",
                        "Resources/Shaders/DisplayCharacter.frag")
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

    d_characterShader.bind();
    d_characterShader.loadUniform("projection", projection);
    d_characterShader.unbind();
}

void DisplayRenderer::draw(const Quad& quad, const QuadVisuals& visuals) const
{
    handleRenderOptions({false, false, false});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto position3D = Maths::vec3{quad.position.x, quad.position.y, 0.0f};
    auto transform = Maths::transform(position3D, {0.0, 0.0, 0.0}, {quad.width, quad.height, 0.0});

    // Find the appropriate shader and bind the colour/texture.
    d_colourShader.bind();

    d_colourShader.loadUniform("colour", visuals.colour);
    d_colourShader.loadUniform("transform", transform);
    d_colourShader.loadUniform("opacity", visuals.opacity);
    d_colourShader.loadUniform("roundness", visuals.roundness);
    d_colourShader.loadUniform("greyscale", visuals.greyscale ? 1.0f : 0.0f);

    visuals.texture.bind();
    d_quad.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    d_quad.unbind();
    visuals.texture.unbind();

    d_colourShader.unbind();
    glDisable(GL_BLEND);
}

void DisplayRenderer::draw(int character,
                           const Font& font,
                           const Maths::vec2& lineStart,
                           float size,
                           const Maths::vec3& colour) const
{
    handleRenderOptions({false, false, false});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Character c = font.get(character);

    d_characterShader.bind();

    auto transform = Maths::transform(
        {lineStart.x + c.xOffset(), lineStart.y - c.yOffset(), 0.0f},
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 0.0});

    d_characterShader.loadUniform("transform", transform);
    d_characterShader.loadUniform("colour", colour);

    c.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    c.unbind();
    d_characterShader.bind();
}

void DisplayRenderer::draw(const std::string& sentence,
                           const Font& font,
                           const Maths::vec2& lineStart,
                           float size,
                           const Maths::vec3& colour) const
{
    handleRenderOptions({false, false, false});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Maths::vec3 pointer = {lineStart.x, lineStart.y, 0.0f};

    d_characterShader.bind();
    d_characterShader.loadUniform("colour", colour);

    for (int character : sentence) {
        Character c = font.get(character);

        auto transform = Maths::transform(
            pointer + c.offset(),
            {0.0, 0.0, 0.0},
            {1.0, 1.0, 0.0});

        d_characterShader.loadUniform("transform", transform);
        d_characterShader.loadUniform("colour", colour);

        c.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        c.unbind();

        pointer.x += c.advance();
    }

    d_characterShader.bind();
}

}