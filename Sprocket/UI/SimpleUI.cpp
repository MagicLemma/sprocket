#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"
#include "KeyboardCodes.h"
#include "Log.h"
#include "Maths.h"
#include "RenderContext.h"
#include "BufferLayout.h"
#include "Adaptors.h"

#include <functional>
#include <sstream>
#include <cassert>
#include <algorithm>

namespace Sprocket {
namespace {

struct TextInfo
{
    float width = 0.0f;
    float height = 0.0f;
};

TextInfo GetTextInfo(Font& font, float size, const std::string& text)
{
    TextInfo info;
    for (char c : text) { // TODO: Take kerning into account.
        auto glyph = font.GetGlyph(c, size);
        info.width += glyph.advance.x;
        if (glyph.height > info.height) {
            info.height = (float)glyph.height;
        }
    }

    Glyph first = font.GetGlyph(text.front(), size);
    info.width -= first.offset.x;

    Glyph last = font.GetGlyph(text.back(), size);
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
    
    if (info.mouseOver) {
        float r = std::min(info.mouseOver, interval) / (float)interval;
        ret = (1 - r) * ret + r * hovered;
    } else {
        float r = std::min(info.sinceUnhovered, interval) / (float)interval;
        ret = (1 - r) * hovered + r * ret;
    }

    if (info.mouseDown) {
        float r = std::min(info.mouseDown, interval) / (float)interval;
        ret = (1 - r) * ret + r * clicked;
    } else {
        float r = std::min(info.sinceUnlicked, interval) / (float)interval;
        ret = (1 - r) * clicked + r * ret;
    }

    return ret;
}

}

SimpleUI::SimpleUI(Window* window)
    : d_window(window)
    , d_engine(window)
{
    d_keyboard.ConsumeAll(false);
}

void SimpleUI::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
    d_engine.OnEvent(event);
}

void SimpleUI::OnUpdate(double dt)
{
    d_mouse.OnUpdate();
    d_engine.OnUpdate(dt);
}

void SimpleUI::StartFrame()
{
    d_engine.StartFrame();
}

void SimpleUI::EndFrame()
{
    d_engine.EndFrame();
}

bool SimpleUI::StartPanel(
    const std::string& name,
    Maths::vec4* region,
    bool* active,
    bool* draggable)
{
    d_engine.StartPanel(name, region, active, draggable);

    if(*active) {
        float thickness = 5.0f;
        auto border = *region;
        border.x -= thickness;
        border.y -= thickness;
        border.z += 2.0f * thickness;
        border.w += 2.0f * thickness;

        d_engine.DrawQuad(d_theme.backgroundColour * 0.35f, border);
        d_engine.DrawQuad(d_theme.backgroundColour * 0.7f, *region);
    }

    return *active;
}

void SimpleUI::EndPanel()
{
    d_engine.EndPanel();
}

void SimpleUI::Quad(const Maths::vec4& colour, const Maths::vec4& quad)
{
    auto copy = d_engine.ApplyOffset(quad);
    d_engine.DrawQuad(colour, copy);
}

void SimpleUI::Text(
    const std::string& text,
    float size,
    const Maths::vec4& quad)
{
    auto copy = d_engine.ApplyOffset(quad);
    d_engine.DrawText(text, size, copy);
}

bool SimpleUI::Button(const std::string& name,
                      const Maths::vec4& region)
{
    Maths::vec4 copy = d_engine.ApplyOffset(region);
    auto info = d_engine.RegisterWidget(name, copy);

    Maths::vec4 hoveredRegion = copy;
    hoveredRegion.x -= 10.0f;
    hoveredRegion.z += 20.0f;

    Maths::vec4 clickedRegion = copy;
    clickedRegion.x += 10.0f;
    clickedRegion.z -= 20.0f;

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 shape = Interpolate(info, copy, hoveredRegion, clickedRegion);
    
    d_engine.DrawQuad(colour, shape);
    d_engine.DrawText(name, 36.0f, copy);

    return info.onClick;
}

bool SimpleUI::Checkbox(const std::string& name,
                        const Maths::vec4& region,
                        bool* value)
{
    Maths::vec4 copy = d_engine.ApplyOffset(region);
    auto info = d_engine.RegisterWidget(name, copy);

    auto unselected = Interpolate(info, d_theme.backgroundColour, d_theme.backgroundColour*1.1f, d_theme.clickedColour);
    auto selected = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);;

    float r = std::min(info.sinceClicked, 0.1) / 0.1f;

    Maths::vec4 colour;
    if (*value) {
        colour = r * selected + (1 - r) * unselected;
    } else {
        colour = r * unselected + (1 - r) * selected;
    }

    if (info.onClick) {
        *value = !(*value);
    }

    d_engine.DrawQuad(colour, copy);
    d_engine.DrawText(name, 36.0f, copy);

    return *value; 
}

void SimpleUI::Slider(const std::string& name,
                      const Maths::vec4& region,
                      float* value, float min, float max)
{
    Maths::vec4 copy = d_engine.ApplyOffset(region);
    auto info = d_engine.RegisterWidget(name, copy);

    float x = copy.x;
    float y = copy.y;
    float width = copy.z;
    float height = copy.w;

    Maths::vec4 leftColour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 rightColour = d_theme.backgroundColour;
    
    float ratio = (*value - min) / (max - min);
    d_engine.DrawQuad(leftColour, {x, y, ratio * width, height});
    d_engine.DrawQuad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    d_engine.DrawText(name + ": " + Maths::ToString(*value, 0), 36.0f, copy);

    if (info.mouseDown) {
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
    Maths::vec4 copy = d_engine.ApplyOffset(region);
    auto info = d_engine.RegisterWidget(name, copy);

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    
    d_engine.DrawQuad(colour, copy);
    d_engine.DrawText(name + ": " + Maths::ToString(*value, 0), 36.0f, copy);

    if (info.mouseDown) {
        *value += d_mouse.GetMouseOffset().x * speed;
    }    
}

void SimpleUI::Image(const std::string& name,
                     const Texture& image,
                     const Maths::vec2& position)
{
    Maths::vec4 region{position.x, position.y, image.Width(), image.Height()};
    Maths::vec4 copy = d_engine.ApplyOffset(region);

    DrawCommand cmd;
    cmd.vertices = {
        {{copy.x,          copy.y         }, {1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}},
        {{copy.x + copy.z, copy.y         }, {1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}},
        {{copy.x,          copy.y + copy.w}, {1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}},
        {{copy.x + copy.z, copy.y + copy.w}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}}
    };
    cmd.indices = {0, 1, 2, 2, 1, 3};
    cmd.texture = &image;
    d_engine.SubmitDrawCommand(cmd);
}

}