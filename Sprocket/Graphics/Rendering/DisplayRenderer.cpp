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
    , d_characterShader("Resources/Shaders/DisplayCharacter.vert",
                        "Resources/Shaders/DisplayCharacter.frag")
    , d_quad(GetQuad())
    , d_whiteTexture("Resources/Textures/White.png")
{
    d_availableFonts.insert({FontEnum::ARIAL, {"Resources/Fonts/Arial.fnt",
                                           "Resources/Fonts/Arial.png"}});
    d_availableFonts.insert({FontEnum::GEORGIA, {"Resources/Fonts/Georgia.fnt",
                                             "Resources/Fonts/Georgia.png"}});
    d_availableFonts.insert({FontEnum::CALIBRI, {"Resources/Fonts/Calibri.fnt",
                                             "Resources/Fonts/Calibri.png"}});
}

FontPackage DisplayRenderer::GetFont(FontEnum font)
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

void DisplayRenderer::OnUpdate() const
{
    float width = (float)d_window->Width();
    float height = (float)d_window->Height();
    Maths::mat4 projection = Maths::Ortho(0, width, height, 0);

    d_colourShader.Bind();
    d_colourShader.LoadUniform("projection", projection);

    d_characterShader.Bind();
    d_characterShader.LoadUniform("projection", projection);
    d_characterShader.Unbind();
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

void DisplayRenderer::Draw(const Text& text)
{
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FontPackage fontPack = GetFont(text.font);
    float fontSize = text.size / fontPack.Size();

    Maths::vec3 pointer = {
        text.position.x,
        text.position.y + text.size,
        0.0f
    };

    d_characterShader.Bind();
    d_characterShader.LoadUniform("colour", text.colour);

    fontPack.Atlas().Bind();
    for (int character : text.message) {
        GlyphStruct c = fontPack.Get(character);

        float xPos = pointer.x + c.xOffset * fontSize;
        float yPos = pointer.y - (c.height - c.yOffset) * fontSize;

        auto transform = Maths::Transform(
            Maths::vec3{xPos, yPos, 0.0f},
            Maths::identity);
        transform = Maths::Scale(transform, fontSize);

        d_characterShader.LoadUniform("transform", transform);

        c.model.Bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        c.model.Unbind();

        pointer.x += c.advance * fontSize;
    }
    fontPack.Atlas().Unbind();

    d_characterShader.Bind();
    glDisable(GL_BLEND);
}

}