#include "DisplayRenderer.h"
#include "Maths.h"
#include "Log.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Sprocket {
namespace {

Vertex2DBuffer getQuad()
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
    , d_characterShader("Resources/Shaders/DisplayCharacter.vert",
                        "Resources/Shaders/DisplayCharacter.frag")
    , d_quad(getQuad())
    , d_whiteTexture("Resources/Textures/White.png")
{
    d_availableFonts.insert({Font::ARIAL, {"Resources/Fonts/Arial.fnt",
                                           "Resources/Fonts/Arial.png"}});
    d_availableFonts.insert({Font::GEORGIA, {"Resources/Fonts/Georgia.fnt",
                                             "Resources/Fonts/Georgia.png"}});
    d_availableFonts.insert({Font::CALIBRI, {"Resources/Fonts/Calibri.fnt",
                                             "Resources/Fonts/Calibri.png"}});
}

FontPackage DisplayRenderer::getFont(Font font)
{
    auto it = d_fonts.find(font);
    if (it != d_fonts.end()) {
        return it->second;
    }

    auto it2 = d_availableFonts.find(font);
    if (it2 == d_availableFonts.end()) {
        SPKT_LOG_ERROR("Font is not available!");
    }

    SPKT_LOG_INFO("Loading a font!");
    auto val = d_fonts.emplace(font, FontPackage(it2->second.first, it2->second.second));
    return val.first->second;
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

void DisplayRenderer::draw(const Quad& quad) const
{
    draw(quad, d_quad);
}

void DisplayRenderer::draw(const Quad& quad, const Model2D& model) const
{
    handleRenderOptions({false, false, false});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto position3D = Maths::vec3{quad.position.x, quad.position.y, 0.0f};
    auto transform = Maths::transform(position3D, {0.0, 0.0, 0.0}, {quad.width, quad.height, 0.0});

    // Find the appropriate shader and bind the colour/texture.
    d_colourShader.bind();

    d_colourShader.loadUniform("colour", quad.colour);
    d_colourShader.loadUniform("transform", transform);
    d_colourShader.loadUniform("opacity", quad.opacity);
    d_colourShader.loadUniform("roundness", quad.roundness);
    d_colourShader.loadUniform("greyscale", quad.greyscale ? 1.0f : 0.0f);

    quad.texture.bind();
    model.bind();
    glDrawArrays(GL_TRIANGLE_STRIP
    , 0, 4);
    model.unbind();
    quad.texture.unbind();

    d_colourShader.unbind();
    glDisable(GL_BLEND);
}

void DisplayRenderer::draw(const Text& text)
{
    handleRenderOptions({false, false, false});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FontPackage fontPack = getFont(text.font);
    float fontSize = text.size / fontPack.size();

    Maths::vec3 pointer = {text.position.x, text.position.y, 0.0f};

    d_characterShader.bind();
    d_characterShader.loadUniform("colour", text.colour);

    for (int character : text.message) {
        Character c = fontPack.get(character);

        float xPos = pointer.x + c.xOffset() * fontSize;
        float yPos = pointer.y - (c.height() - c.yOffset()) * fontSize;

        auto transform = Maths::transform(
            Maths::vec3{xPos, yPos, 0.0f},
            {0.0, 0.0, 0.0},
            fontSize);

        d_characterShader.loadUniform("transform", transform);;

        c.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        c.unbind();

        pointer.x += c.advance() * fontSize;
    }

    d_characterShader.bind();
}

}