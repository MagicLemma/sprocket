#include "SinglePanelUI.h"

#include <Sprocket/Core/Window.h>

#include <functional>
#include <cassert>
#include <algorithm>
#include <format>

namespace spkt {
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
    , d_engine(window)
    , d_font("Resources/Fonts/Coolvetica.ttf")
{
}

void SinglePanelUI::on_event(event& event)
{
    d_engine.on_event(event);
}

void SinglePanelUI::on_update(double dt)
{
    d_engine.on_update(dt);
}

void SinglePanelUI::StartFrame()
{
    std::string name = "__SinglePanelUI_Name__";
    glm::vec4 region = {0, 0, d_window->Width(), d_window->Height()};

    d_engine.StartFrame();
    d_engine.StartPanel(name, &region, PanelType::CLICKABLE);
    d_engine.GetDrawCommand().font = &d_font;
}

void SinglePanelUI::EndFrame()
{
    d_engine.EndPanel();
    d_engine.EndFrame();
}

bool SinglePanelUI::Button(std::string_view name, const glm::vec4& region)
{
    auto info = d_engine.Register(name, region);

    glm::vec4 hoveredRegion = info.quad;
    hoveredRegion.x -= 10.0f;
    hoveredRegion.z += 20.0f;

    glm::vec4 clickedRegion = info.quad;
    clickedRegion.x += 10.0f;
    clickedRegion.z -= 20.0f;

    glm::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    glm::vec4 shape = Interpolate(info, info.quad, hoveredRegion, clickedRegion);
    
    TextProperties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour, shape);
    cmd.AddText(name, info.quad, tp);

    return info.onClick;
}

void SinglePanelUI::Slider(std::string_view name,
                           const glm::vec4& region,
                           float* value, float min, float max)
{
    auto info = d_engine.Register(name, region);

    float x = info.quad.x;
    float y = info.quad.y;
    float width = info.quad.z;
    float height = info.quad.w;
    float ratio = (*value - min) / (max - min);
    glm::vec4 leftColour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    glm::vec4 rightColour = d_theme.backgroundColour;

    TextProperties tp;
    tp.size = 36.0f;
    
    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(leftColour, {x, y, ratio * width, height});
    cmd.AddQuad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    cmd.AddText(std::format("{}: {:.0f}", name, *value), info.quad, tp);

    if (info.sinceClicked > 0) {
        auto mouse = d_window->GetMousePos();
        mouse.x = std::clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SinglePanelUI::Box(const glm::vec4& quad, const glm::vec4& colour)
{
    float padding = 5.0f;
    glm::vec4 border = quad;
    border.x -= padding;
    border.y -= padding;
    border.z += 2 * padding;
    border.w += 2 * padding;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour * 0.7f, border);
    cmd.AddQuad(colour, quad);
}

}