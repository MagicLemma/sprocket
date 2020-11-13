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
    
    if (info.sinceHovered > 0) {
        float r = std::min(info.sinceHovered, interval) / (float)interval;
        ret = (1 - r) * ret + r * hovered;
    } else {
        float r = std::min(info.sinceUnhovered, interval) / (float)interval;
        ret = (1 - r) * hovered + r * ret;
    }

    if (info.sinceClicked > 0) {
        float r = std::min(info.sinceClicked, interval) / (float)interval;
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
    , d_engine(window, &d_keyboard, &d_mouse)
    , d_font("Resources/Fonts/Coolvetica.ttf")
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

void SinglePanelUI::StartFrame()
{
    std::string name = "__SinglePanelUI_Name__";
    Maths::vec4 region = {0, 0, d_window->Width(), d_window->Height()};

    d_engine.StartFrame();
    d_engine.StartPanel(name, &region, PanelType::CLICKABLE);
    d_engine.GetDrawCommand().font = &d_font;
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
    
    TextProperties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour, shape);
    cmd.AddText(name, info.quad, tp);

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
    float ratio = (*value - min) / (max - min);
    Maths::vec4 leftColour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    Maths::vec4 rightColour = d_theme.backgroundColour;

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

void SinglePanelUI::Box(const Maths::vec4& quad, const Maths::vec4& colour)
{
    float padding = 5.0f;
    Maths::vec4 border = quad;
    border.x -= padding;
    border.y -= padding;
    border.z += 2 * padding;
    border.w += 2 * padding;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour * 0.7f, border);
    cmd.AddQuad(colour, quad);
}

}