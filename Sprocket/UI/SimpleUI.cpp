#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "Log.h"
#include "Maths.h"
#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

float TextWidth(const std::string& text, const FontPackage& font, float size)
{
    float textWidth = 0.0f;

    for (char character : text) {
        Glyph c = font.Get(character);
        textWidth += c.advance;
    }

    auto first = font.Get(text.front());
    textWidth -= first.xOffset;

    auto last = font.Get(text.back());
    textWidth += (last.xOffset + last.width);

    return textWidth * size;
}

}

SimpleUI::SimpleUI(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_bufferLayout(sizeof(BufferVertex))
    , d_font("Resources/Fonts/Calibri.fnt",
             "Resources/Fonts/Calibri.png")
{
    d_keyboard.ConsumeAll(false);

    d_bufferLayout.AddAttribute(DataType::FLOAT, 2);
    d_bufferLayout.AddAttribute(DataType::FLOAT, 4);
    d_bufferLayout.AddAttribute(DataType::FLOAT, 2);
    d_buffer.SetBufferLayout(d_bufferLayout);

    d_texAtlas = ftgl::texture_atlas_new(1024, 1024, 1);
    d_texFont = ftgl::texture_font_new_from_file(d_texAtlas, 36.0f, "Resources/Fonts/arial.ttf");

    d_texFont->rendermode = ftgl::RenderMode::RENDER_NORMAL;
    d_texFont->outline_thickness = 0;

    glGenTextures(1, &d_texAtlas->id);
    glBindTexture( GL_TEXTURE_2D, d_texAtlas->id );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, d_texAtlas->width, d_texAtlas->height,
                  0, GL_RED, GL_UNSIGNED_BYTE, (void*)d_texAtlas->data.data() );
    glBindTexture(GL_TEXTURE_2D, 0);
}

void SimpleUI::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
}

void SimpleUI::OnUpdate(double dt)
{
    d_mouse.OnUpdate();

    if (d_mouse.IsButtonReleased(Mouse::LEFT)) {
        d_clicked = -1;
    }
}

void SimpleUI::StartFrame()
{
    d_quadVertices.clear();
    d_quadIndices.clear();

    d_textVertices.clear();
    d_textIndices.clear();
}

void SimpleUI::EndFrame()
{
    Sprocket::RenderContext rc;  
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Maths::mat4 proj = Maths::Ortho(0, d_window->Width(), d_window->Height(), 0);
    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);
    d_buffer.Bind();

    Texture::White().Bind();
    d_buffer.SetVertexData(
        sizeof(BufferVertex) * d_quadVertices.size(),
        d_quadVertices.data());
    d_buffer.SetIndexData(
        sizeof(unsigned int) * d_quadIndices.size(),
        d_quadIndices.data());
    glDrawElements(GL_TRIANGLES, (int)d_quadIndices.size(), GL_UNSIGNED_INT, nullptr);

    glBindTexture(GL_TEXTURE_2D, d_texAtlas->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, d_texAtlas->width, d_texAtlas->height,
                 0, GL_RED, GL_UNSIGNED_BYTE, (void*)d_texAtlas->data.data());

    d_buffer.SetVertexData(
        sizeof(BufferVertex) * d_textVertices.size(),
        d_textVertices.data());
    d_buffer.SetIndexData(
        sizeof(unsigned int) * d_textIndices.size(),
        d_textIndices.data());
    glDrawElements(GL_TRIANGLES, (int)d_textIndices.size(), GL_UNSIGNED_INT, nullptr);
    
    d_buffer.Unbind();
    d_font.Atlas().Unbind();
    
}

void SimpleUI::Quad(float x, float y,
                    float width, float height,
                    const Maths::vec4& colour)
{
    unsigned int index = d_quadVertices.size();
    d_quadVertices.push_back({{x,         y},          colour});
    d_quadVertices.push_back({{x + width, y},          colour});
    d_quadVertices.push_back({{x,         y + height}, colour});
    d_quadVertices.push_back({{x + width, y + height}, colour});

    d_quadIndices.push_back(index + 0);
    d_quadIndices.push_back(index + 1);
    d_quadIndices.push_back(index + 2);
    d_quadIndices.push_back(index + 2);
    d_quadIndices.push_back(index + 1);
    d_quadIndices.push_back(index + 3);
}

bool SimpleUI::Button(
    int id, const std::string& name,
    float x, float y,
    float width, float height)
{
    auto mouse = d_mouse.GetMousePos();
    auto hovered = d_mouse.InRegion(x, y, width, height);
    auto clicked = hovered && d_mouse.IsButtonClicked(Mouse::LEFT);
    if (clicked) { d_clicked = id; }

    Maths::vec4 colour = d_theme.baseColour;
    if (d_clicked == id) {
        colour = d_theme.clickedColour;
    }
    else if (hovered) {
        colour = d_theme.hoveredColour;
    }

    Quad(x, y, width, height, colour);
    AddText(x, y, name, 0.6f * height, width);
    return clicked;
}

void SimpleUI::Slider(int id, const std::string& name,
                      float x, float y, float width, float height,
                      float* value, float min, float max)
{
    auto mouse = d_mouse.GetMousePos();
    auto hovered = d_mouse.InRegion(x, y, width, height);
    auto clicked = hovered && d_mouse.IsButtonClicked(Mouse::LEFT);
    if (clicked) { d_clicked = id; }

    float ratio = (*value - min) / (max - min);
    Quad(x, y, ratio * width, height, d_theme.hoveredColour);
    Quad(x + ratio * width, y, (1 - ratio) * width, height, d_theme.baseColour);
    
    std::stringstream text;
    text << name << ": " << Maths::ToString(*value, 0);
    
    AddText(x, y, text.str(), 0.6f * height, width);

    if (d_clicked == id) {
        Maths::Clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SimpleUI::AddText(float x, float y, const std::string& text, float size, float width)
{
    Maths::vec4 colour = {1.0, 1.0, 1.0, 1.0};
    float fontSize = 1.0f;

    float textWidth = TextWidth(text, d_font, fontSize);
    Maths::vec2 pointer(x + (width - textWidth) / 2.0f, y);
    pointer = {x, y};
    
    for (std::size_t i = 0; i != text.size(); ++i) {
        char c = text[i];
        auto glyph = ftgl::texture_font_get_glyph(d_texFont, &c);

        float kerning = 0.0f;
        if (i > 0) {
            kerning = ftgl::texture_glyph_get_kerning(glyph, &text[i-1]);
        }
        pointer.x += kerning;

        float xPos = pointer.x + glyph->offset_x * fontSize;
        float yPos = pointer.y - glyph->offset_y * fontSize;

        float width = glyph->width * fontSize;
        float height = glyph->height * fontSize;

        float x = glyph->s0;
        float y = glyph->t0;
        float w = glyph->s1 - glyph->s0;
        float h = glyph->t1 - glyph->t0;

        pointer.x += glyph->advance_x * fontSize;

        unsigned int index = d_textVertices.size();
        d_textVertices.push_back({{xPos,         yPos},          colour, {x,     y    }});
        d_textVertices.push_back({{xPos + width, yPos},          colour, {x + w, y    }});
        d_textVertices.push_back({{xPos,         yPos + height}, colour, {x,     y + h}});
        d_textVertices.push_back({{xPos + width, yPos + height}, colour, {x + w, y + h}});

        d_textIndices.push_back(index + 0);
        d_textIndices.push_back(index + 1);
        d_textIndices.push_back(index + 2);
        d_textIndices.push_back(index + 2);
        d_textIndices.push_back(index + 1);
        d_textIndices.push_back(index + 3);
    }
}

}