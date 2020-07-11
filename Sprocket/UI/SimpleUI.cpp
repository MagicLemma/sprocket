#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "Log.h"
#include "Maths.h"
#include "RenderContext.h"

namespace Sprocket {
namespace {

struct TextInfo
{
    float width = 0.0f;
    float height = 0.0f;
};

TextInfo GetTextInfo(Font& font, const std::string& text)
{
    TextInfo info;
    for (char c : text) {
        auto glyph = font.GetGlyph(c);
        info.width += glyph.advance.x;
        if (glyph.height > info.height) {
            info.height = glyph.height;
        }
    }

    Glyph first = font.GetGlyph(text.front());
    info.width -= first.offset.x;

    Glyph last = font.GetGlyph(text.back());
    info.width += last.width;
    info.width += last.offset.x;
    info.width -= last.advance.x;

    return info;
}

}

SimpleUI::SimpleUI(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_bufferLayout(sizeof(BufferVertex))
    , d_font("Resources/Fonts/Coolvetica.ttf", 36.0f)
{
    d_keyboard.ConsumeAll(false);

    d_bufferLayout.AddAttribute(DataType::FLOAT, 2);
    d_bufferLayout.AddAttribute(DataType::FLOAT, 4);
    d_bufferLayout.AddAttribute(DataType::FLOAT, 2);
    d_buffer.SetBufferLayout(d_bufferLayout);
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
    rc.AlphaBlending(true);
    rc.FaceCulling(false);
    rc.DepthTesting(false);

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
    d_buffer.Draw(d_quadIndices.size());

    d_font.Bind();
    d_buffer.SetVertexData(
        sizeof(BufferVertex) * d_textVertices.size(),
        d_textVertices.data());
    d_buffer.SetIndexData(
        sizeof(unsigned int) * d_textIndices.size(),
        d_textIndices.data());
    d_buffer.Draw(d_textIndices.size());

    d_buffer.Unbind();
    
}

void SimpleUI::Quad(const Maths::vec4& colour,
                    const Maths::vec4& region)
{
    float x = region.x;
    float y = region.y;
    float width = region.z;
    float height = region.w;

    unsigned int index = d_quadVertices.size();
    d_quadVertices.push_back({{x,         y},          colour * 1.4f});
    d_quadVertices.push_back({{x + width, y},          colour * 1.4f});
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
    const Maths::vec4& region)
{
    float x = region.x;
    float y = region.y;
    float width = region.z;
    float height = region.w;
    
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

    Quad(colour, {x, y, width, height});
    Text(name, {x, y, width, height});
    return clicked;
}

void SimpleUI::Slider(int id, const std::string& name,
                      const Maths::vec4& region,
                      float* value, float min, float max)
{
    float x = region.x;
    float y = region.y;
    float width = region.z;
    float height = region.w;
    
    auto mouse = d_mouse.GetMousePos();
    auto hovered = d_mouse.InRegion(x, y, width, height);
    auto clicked = hovered && d_mouse.IsButtonClicked(Mouse::LEFT);
    if (clicked) { d_clicked = id; }
    
    Maths::vec4 leftColour = d_theme.baseColour;
    Maths::vec4 rightColour = d_theme.backgroundColour;
    if (d_clicked == id) {
        leftColour = d_theme.clickedColour;
    }
    else if (hovered) {
        leftColour = d_theme.hoveredColour;
    }

    float ratio = (*value - min) / (max - min);
    Quad(leftColour, {x, y, ratio * width, height});
    Quad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    
    std::stringstream text;
    text << name << ": " << Maths::ToString(*value, 0);
    
    Text(text.str(), region);

    if (d_clicked == id) {
        Maths::Clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SimpleUI::Text(
    const std::string& text,
    const Maths::vec4& region)
{
    float x = region.x;
    float y = region.y;
    float width = region.z;
    float height = region.w;
    
    Maths::vec4 colour = {1.0, 1.0, 1.0, 1.0};

    Glyph first = d_font.GetGlyph(text.front());
    auto textInfo = GetTextInfo(d_font, text);

    Maths::vec2 pen = {
        x + (width - textInfo.width) / 2.0f,
        y + (height - first.height) / 2.0f
    };

    pen.x -= first.offset.x;
    pen.y += first.offset.y;
    
    for (std::size_t i = 0; i != text.size(); ++i) {
        auto glyph = d_font.GetGlyph(text[i]);

        if (i > 0) {
            float kerning = d_font.GetKerning(text[i-1], text[i]);
            pen.x += kerning;
        }

        float xPos = pen.x + glyph.offset.x;
        float yPos = pen.y - glyph.offset.y;

        float width = glyph.width;
        float height = glyph.height;

        float x = glyph.texture.x;
        float y = glyph.texture.y;
        float w = glyph.texture.z;
        float h = glyph.texture.w;

        pen += glyph.advance;

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