#include "SinglePanelUI.h"
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

SinglePanelUI::SinglePanelUI(Window* window)
    : d_window(window)
    , d_engine(window)
{
    d_keyboard.ConsumeAll(false);
}

void SinglePanelUI::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
    d_engine.OnEvent(event);
}

void SinglePanelUI::OnUpdate(double dt)
{
    d_mouse.OnUpdate();
    d_engine.OnUpdate(dt);
}

bool SinglePanelUI::StartFrame(
    const std::string& name,
    Maths::vec4* region,
    bool* active,
    bool* draggable)
{
    d_engine.StartFrame();
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

void SinglePanelUI::EndFrame()
{
    d_engine.EndPanel();
    d_engine.EndFrame();
}

bool SinglePanelUI::Button(const std::string& name, const Maths::vec4& region)
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
    
    d_engine.DrawQuad(colour, shape);
    d_engine.DrawText(name, 36.0f, info.quad);

    return info.onClick;
}

void SinglePanelUI::Slider(const std::string& name,
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
    d_engine.DrawQuad(leftColour, {x, y, ratio * width, height});
    d_engine.DrawQuad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    d_engine.DrawText(name + ": " + Maths::ToString(*value, 0), 36.0f, info.quad);

    if (info.mouseDown) {
        auto mouse = d_mouse.GetMousePos();
        Maths::Clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

}