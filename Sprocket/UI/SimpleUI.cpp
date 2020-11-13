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

template <typename T> T Interpolate(
    const WidgetInfo& info,
    const T& base,
    const T& hovered,
    const T& clicked)
{
    T ret = base;
    double interval = 0.1;
    
    if (info.sinceHovered) {
        float r = std::min(info.sinceHovered, interval) / (float)interval;
        ret = (1 - r) * ret + r * hovered;
    } else {
        float r = std::min(info.sinceUnhovered, interval) / (float)interval;
        ret = (1 - r) * hovered + r * ret;
    }

    if (info.sinceClicked) {
        float r = std::min(info.sinceClicked, interval) / (float)interval;
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
    , d_engine(window, &d_keyboard, &d_mouse)
    , d_font("Resources/Fonts/Coolvetica.ttf")
{
    d_keyboard.ConsumeAll(false);
}

void SimpleUI::OnEvent(Event& event)
{
    d_engine.OnEvent(event);
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
}

void SimpleUI::EndFrame()
{
    d_engine.EndFrame();
}

bool SimpleUI::StartPanel(
    const std::string& name,
    Maths::vec4* region,
    bool active,
    bool draggable,
    bool clickable)
{
    d_engine.StartPanel(name, region, active, draggable, clickable);
    d_keyboard.ConsumeAll(false);

    if(active) {
        auto& cmd = d_engine.GetDrawCommand();
        cmd.font = &d_font;
        cmd.AddQuad(d_theme.backgroundColour * 0.7f, *region);
    }

    return active;
}

void SimpleUI::EndPanel()
{
    d_engine.EndPanel();
}

void SimpleUI::Quad(const Maths::vec4& colour, const Maths::vec4& quad)
{
    auto region = d_engine.ApplyOffset(quad);

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour, region);
}

void SimpleUI::Text(
    const std::string& text,
    float size,
    const Maths::vec4& quad,
    const Maths::vec4& colour)
{
    auto region = d_engine.ApplyOffset(quad);

    TextProperties tp;
    tp.colour = colour;
    tp.size = size;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddText(text, region, tp);
}

void SimpleUI::Text(
    const std::string& text,
    float size,
    const Maths::vec2& position,
    const Maths::vec4& colour)
{
    auto region = d_engine.ApplyOffset({position.x, position.y, 0, 0});

    TextProperties tp;
    tp.alignment = Alignment::LEFT;
    tp.colour = colour;
    tp.size = size;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddText(text, region, tp);
}

void SimpleUI::TextModifiable(
    const std::string& name,
    const Maths::vec4& region,
    std::string* text,
    const Maths::vec4& colour)
{
    auto info = d_engine.Register(name, region);

    DrawCommand cmd;
    cmd.region = info.quad;
    cmd.texture = d_engine.GetWhite();
    cmd.font = &d_font;

    for (int key : info.keyPresses) {
        if (key == Keyboard::BACKSPACE) {
            if (text->size() > 0) {
                text->pop_back();
            }
        } else {
            text->push_back(static_cast<char>(key));
        }
    }

    auto boxColour = info.sinceFocused > 0 ? d_theme.hoveredColour : d_theme.baseColour;
    cmd.AddQuad(boxColour, info.quad);

    std::string printText = *text;
    if (info.sinceFocused > 0 && Maths::Modulo(info.sinceFocused, 1.0) < 0.5) {
        printText.push_back('|');
    }

    TextProperties tp;
    tp.alignment = Alignment::LEFT;
    tp.colour = colour;
    tp.size = 36.0f;

    cmd.AddText(printText, info.quad, tp);

    d_engine.SubmitDrawCommand(cmd);
}

bool SimpleUI::Button(const std::string& name, const Maths::vec4& region)
{
    auto info = d_engine.Register(name, region);

    Maths::vec4 hoveredRegion = info.quad;
    hoveredRegion.x -= 10.0f;
    hoveredRegion.z += 20.0f;

    Maths::vec4 clickedRegion = info.quad;
    clickedRegion.x += 10.0f;
    clickedRegion.z -= 20.0f;

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 shape = Interpolate(info, info.quad, hoveredRegion, clickedRegion);

    TextProperties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour, shape);
    cmd.AddText(name, info.quad, tp);

    return info.onClick;
}

bool SimpleUI::Checkbox(const std::string& name,
                        const Maths::vec4& region,
                        bool* value)
{
    auto info = d_engine.Register(name, region);

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

    TextProperties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour, info.quad);
    cmd.AddText(name, info.quad, tp);

    return *value; 
}

void SimpleUI::Slider(const std::string& name,
                      const Maths::vec4& region,
                      float* value, float min, float max)
{
    auto info = d_engine.Register(name, region);

    float x = info.quad.x;
    float y = info.quad.y;
    float width = info.quad.z;
    float height = info.quad.w;
    Maths::vec4 leftColour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 rightColour = d_theme.backgroundColour;
    float ratio = (*value - min) / (max - min);

    TextProperties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(leftColour, {x, y, ratio * width, height});
    cmd.AddQuad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    cmd.AddText(name + ": " + Maths::ToString(*value, 0), info.quad, tp);

    if (info.sinceClicked > 0) {
        auto mouse = d_mouse.GetMousePos();
        mouse.x = std::clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SimpleUI::Dragger(const std::string& name,
                       const Maths::vec4& region,
                       float* value, float speed)
{
    auto info = d_engine.Register(name, region);

    Maths::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    
    TextProperties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour, info.quad);
    cmd.AddText(name + ": " + Maths::ToString(*value, 0), info.quad, tp);

    if (info.sinceClicked > 0) {
        *value += d_mouse.GetMouseOffset().x * speed;
    }    
}

void SimpleUI::Image(const std::string& name,
                     std::shared_ptr<Texture> image,
                     const Maths::vec2& position)
{
    Maths::vec4 region{position.x, position.y, image->Width(), image->Height()};
    Maths::vec4 copy = d_engine.ApplyOffset(region);

    DrawCommand cmd;
    cmd.vertices = {
        {{copy.x,          copy.y         }, {1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}},
        {{copy.x + copy.z, copy.y         }, {1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}},
        {{copy.x,          copy.y + copy.w}, {1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}},
        {{copy.x + copy.z, copy.y + copy.w}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}}
    };
    cmd.indices = {0, 1, 2, 2, 1, 3};
    cmd.texture = image.get();
    d_engine.SubmitDrawCommand(cmd);
}

}