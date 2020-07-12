#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "Log.h"
#include "Maths.h"
#include "RenderContext.h"

#include <functional>
#include <sstream>

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
            info.height = (float)glyph.height;
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

template <typename T> T Interpolate(
    const WidgetInfo& info,
    const T& base,
    const T& hovered,
    const T& clicked)
{
    T ret = base;
    double interval = 0.1;
    
    if (info.hovered) {
        float r = std::min(info.hovered, interval) / (float)interval;
        ret = (1 - r) * ret + r * hovered;
    } else {
        float r = std::min(info.unhovered, interval) / (float)interval;
        ret = (1 - r) * hovered + r * ret;
    }

    if (info.clicked) {
        float r = std::min(info.clicked, interval) / (float)interval;
        ret = (1 - r) * ret + r * clicked;
    } else {
        float r = std::min(info.unclicked, interval) / (float)interval;
        ret = (1 - r) * clicked + r * ret;
    }

    return ret;
}

}

SimpleUI::SimpleUI(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_bufferLayout(sizeof(BufferVertex))
    , d_font("Resources/Fonts/Coolvetica.ttf", 36.0f)
    , d_engine(&d_keyboard, &d_mouse)
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
    d_engine.OnUpdate(dt);
}

void SimpleUI::StartFrame()
{
    d_engine.StartFrame();

    d_quadVertices.clear();
    d_quadIndices.clear();

    d_textVertices.clear();
    d_textIndices.clear();
}

void SimpleUI::EndFrame()
{
    d_engine.EndFrame();

    Sprocket::RenderContext rc;
    rc.AlphaBlending(true);
    rc.FaceCulling(false);
    rc.DepthTesting(false);

    float w = (float)d_window->Width();
    float h = (float)d_window->Height();
    auto proj = Maths::Ortho(0, w, h, 0);
    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);

    d_buffer.Bind();
    Texture::White().Bind();
    d_buffer.Draw(d_quadVertices, d_quadIndices);
    d_font.Bind();
    d_buffer.Draw(d_textVertices, d_textIndices);
    d_buffer.Unbind();
}

void SimpleUI::Quad(const Maths::vec4& colour,
                    const Maths::vec4& region)
{
    float x = region.x;
    float y = region.y;
    float width = region.z;
    float height = region.w;

    std::size_t index = d_quadVertices.size();
    d_quadVertices.push_back({{x,         y},          colour});
    d_quadVertices.push_back({{x + width, y},          colour});
    d_quadVertices.push_back({{x,         y + height}, colour * 0.8f});
    d_quadVertices.push_back({{x + width, y + height}, colour * 0.8f});

    d_quadIndices.push_back(index + 0);
    d_quadIndices.push_back(index + 1);
    d_quadIndices.push_back(index + 2);
    d_quadIndices.push_back(index + 2);
    d_quadIndices.push_back(index + 1);
    d_quadIndices.push_back(index + 3);
}

bool SimpleUI::Button(const std::string& name,
                      const Maths::vec4& region)
{
    auto info = d_engine.RegisterWidget(name, region);

    Maths::vec4 hoveredRegion = region;
    hoveredRegion.x -= 10.0f;
    hoveredRegion.z += 20.0f;

    Maths::vec4 clickedRegion = region;
    clickedRegion.x += 10.0f;
    clickedRegion.z -= 20.0f;

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 shape = Interpolate(info, region, hoveredRegion, clickedRegion);
    
    Quad(colour, shape);
    Text(name, region);

    return info.onClick;
}

void SimpleUI::Slider(const std::string& name,
                      const Maths::vec4& region,
                      float* value, float min, float max)
{
    auto info = d_engine.RegisterWidget(name, region);

    float x = region.x;
    float y = region.y;
    float width = region.z;
    float height = region.w;

    Maths::vec4 leftColour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 rightColour = d_theme.backgroundColour;
    
    float ratio = (*value - min) / (max - min);
    Quad(leftColour, {x, y, ratio * width, height});
    Quad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    
    std::stringstream text;
    text << name << ": " << Maths::ToString(*value, 0);
    Text(text.str(), region);

    if (info.clicked) {
        auto mouse = d_mouse.GetMousePos();
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
            pen.x += d_font.GetKerning(text[i-1], text[i]);
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