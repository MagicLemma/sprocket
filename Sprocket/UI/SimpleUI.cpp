#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "KeyboardCodes.h"
#include "Log.h"
#include "Maths.h"
#include "RenderContext.h"
#include "BufferLayout.h"

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
    , d_font("Resources/Fonts/Coolvetica.ttf", 36.0f)
    , d_engine(&d_keyboard, &d_mouse)
{
    d_keyboard.ConsumeAll(false);

    BufferLayout layout(sizeof(BufferVertex));
    layout.AddAttribute(DataType::FLOAT, 2);
    layout.AddAttribute(DataType::FLOAT, 4);
    layout.AddAttribute(DataType::FLOAT, 2);
    d_buffer.SetBufferLayout(layout);
}

Maths::vec4 SimpleUI::ApplyOffset(const Maths::vec4& region)
{
    if (d_offset.size() == 0) {
        return region;
    }

    Maths::vec4 quad = region;
    quad.x += d_offset.top().x;
    quad.y += d_offset.top().y;
    return quad;
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

    d_commands.clear();
    d_commands.push_back(DrawCommand());
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
    for (const auto& cmd : d_commands) {
        Texture::White().Bind();
        d_buffer.Draw(cmd.quadVertices, cmd.quadIndices);
        d_font.Bind();
        d_buffer.Draw(cmd.textVertices, cmd.textIndices);
    }
    d_buffer.Unbind();
}

void SimpleUI::StartWindow(const std::string& name, Maths::vec4* region)
{
    auto copy = ApplyOffset(*region);

    ++d_commandIndex;
    if (d_commandIndex == d_commands.size()) {
        d_commands.push_back(DrawCommand());
    }

    auto info = d_engine.RegisterWidget(name, copy);

    if (info.clicked) {
        region->x += d_mouse.GetMouseOffset().x;
        region->y += d_mouse.GetMouseOffset().y;
    }

    DrawQuad(d_theme.backgroundColour, copy);

    d_offset.push({copy.x, copy.y});
}

void SimpleUI::EndWindow()
{
    --d_commandIndex;
    d_offset.pop();
}

void SimpleUI::DrawQuad(const Maths::vec4& colour,
                    const Maths::vec4& region)
{
    auto copy = region;
    float x = copy.x;
    float y = copy.y;
    float width = copy.z;
    float height = copy.w;

    auto& cmd = d_commands[d_commandIndex];
    std::size_t index = cmd.quadVertices.size();
    cmd.quadVertices.push_back({{x,         y},          colour});
    cmd.quadVertices.push_back({{x + width, y},          colour});
    cmd.quadVertices.push_back({{x,         y + height}, colour * 0.8f});
    cmd.quadVertices.push_back({{x + width, y + height}, colour * 0.8f});

    cmd.quadIndices.push_back(index + 0);
    cmd.quadIndices.push_back(index + 1);
    cmd.quadIndices.push_back(index + 2);
    cmd.quadIndices.push_back(index + 2);
    cmd.quadIndices.push_back(index + 1);
    cmd.quadIndices.push_back(index + 3);
}

void SimpleUI::DrawText(
    const std::string& text,
    const Maths::vec4& region)
{
    auto copy = region;
    Maths::vec4 colour = {1.0, 1.0, 1.0, 1.0};

    Glyph first = d_font.GetGlyph(text.front());
    auto textInfo = GetTextInfo(d_font, text);

    Maths::vec2 pen = {
        region.x + (copy.z - textInfo.width) / 2.0f,
        region.y + (copy.w - first.height) / 2.0f
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

        auto& cmd = d_commands[d_commandIndex];
        unsigned int index = cmd.textVertices.size();
        cmd.textVertices.push_back({{xPos,         yPos},          colour, {x,     y    }});
        cmd.textVertices.push_back({{xPos + width, yPos},          colour, {x + w, y    }});
        cmd.textVertices.push_back({{xPos,         yPos + height}, colour, {x,     y + h}});
        cmd.textVertices.push_back({{xPos + width, yPos + height}, colour, {x + w, y + h}});

        cmd.textIndices.push_back(index + 0);
        cmd.textIndices.push_back(index + 1);
        cmd.textIndices.push_back(index + 2);
        cmd.textIndices.push_back(index + 2);
        cmd.textIndices.push_back(index + 1);
        cmd.textIndices.push_back(index + 3);
    }
}

void SimpleUI::Quad(const Maths::vec4& colour, const Maths::vec4& quad)
{
    auto copy = ApplyOffset(quad);
    DrawQuad(colour, copy);
}

void SimpleUI::Text(const std::string& text, const Maths::vec4& quad)
{
    auto copy = ApplyOffset(quad);
    DrawText(text, copy);
}

bool SimpleUI::Button(const std::string& name,
                      const Maths::vec4& region)
{
    Maths::vec4 copy = ApplyOffset(region);
    auto info = d_engine.RegisterWidget(name, copy);

    Maths::vec4 hoveredRegion = copy;
    hoveredRegion.x -= 10.0f;
    hoveredRegion.z += 20.0f;

    Maths::vec4 clickedRegion = copy;
    clickedRegion.x += 10.0f;
    clickedRegion.z -= 20.0f;

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 shape = Interpolate(info, copy, hoveredRegion, clickedRegion);
    
    DrawQuad(colour, shape);
    DrawText(name, copy);

    return info.onClick;
}

void SimpleUI::Slider(const std::string& name,
                      const Maths::vec4& region,
                      float* value, float min, float max)
{
    Maths::vec4 copy = ApplyOffset(region);
    auto info = d_engine.RegisterWidget(name, copy);

    float x = copy.x;
    float y = copy.y;
    float width = copy.z;
    float height = copy.w;

    Maths::vec4 leftColour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 rightColour = d_theme.backgroundColour;
    
    float ratio = (*value - min) / (max - min);
    DrawQuad(leftColour, {x, y, ratio * width, height});
    DrawQuad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    DrawText(name + ": " + Maths::ToString(*value, 0), copy);

    if (info.clicked) {
        auto mouse = d_mouse.GetMousePos();
        Maths::Clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SimpleUI::Dragger(const std::string& name,
                       const Maths::vec4& region,
                       float* value, float speed)
{
    Maths::vec4 copy = ApplyOffset(region);
    auto info = d_engine.RegisterWidget(name, copy);

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    
    DrawQuad(colour, copy);
    DrawText(name + ": " + Maths::ToString(*value, 0), copy);

    if (info.clicked) {
        *value += d_mouse.GetMouseOffset().x * speed;
    }    
}



}