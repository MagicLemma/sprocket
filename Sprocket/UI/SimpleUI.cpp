#include "SimpleUI.h"

#include <Sprocket/Core/window.h>
#include <Sprocket/Core/events.h>
#include <Sprocket/Core/input_codes.h>

#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <functional>
#include <string_view>

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

SimpleUI::SimpleUI(window* window)
    : d_window(window)
    , d_engine(window)
    , d_font("Resources/Fonts/Coolvetica.ttf")
{
}

void SimpleUI::on_event(event& event)
{
    d_engine.on_event(event);
}

void SimpleUI::on_update(double dt)
{
    d_engine.on_update(dt);
}

void SimpleUI::StartFrame()
{
    d_engine.StartFrame();
}

void SimpleUI::EndFrame()
{
    d_engine.EndFrame();
}

void SimpleUI::StartPanel(std::string_view name, glm::vec4* region, PanelType type)
{
    d_engine.StartPanel(name, region, type);

    auto& cmd = d_engine.GetDrawCommand();
    cmd.font = &d_font;
    cmd.AddQuad(d_theme.backgroundColour * 0.7f, *region);
}

void SimpleUI::EndPanel()
{
    d_engine.EndPanel();
}

void SimpleUI::Quad(const glm::vec4& colour, const glm::vec4& quad)
{
    auto region = d_engine.ApplyOffset(quad);

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour, region);
}

void SimpleUI::Text(
    std::string_view text,
    float size,
    const glm::vec4& quad,
    const glm::vec4& colour)
{
    auto region = d_engine.ApplyOffset(quad);

    TextProperties tp;
    tp.colour = colour;
    tp.size = size;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddText(text, region, tp);
}

void SimpleUI::Text(
    std::string_view text,
    float size,
    const glm::vec2& position,
    const glm::vec4& colour)
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
    std::string_view name,
    const glm::vec4& region,
    std::string* text,
    const glm::vec4& colour)
{
    auto info = d_engine.Register(name, region);

    DrawCommand cmd;
    cmd.region = info.quad;
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

bool SimpleUI::Button(std::string_view name, const glm::vec4& region)
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

bool SimpleUI::Checkbox(std::string_view name,
                        const glm::vec4& region,
                        bool* value)
{
    auto info = d_engine.Register(name, region);

    auto unselected = Interpolate(info, d_theme.backgroundColour, d_theme.backgroundColour*1.1f, d_theme.clickedColour);
    auto selected = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);;

    float r = std::min(info.sinceClicked, 0.1) / 0.1f;

    glm::vec4 colour;
    if (*value) {
        colour = r * unselected + (1 - r) * selected;
    } else {
        colour = r * selected + (1 - r) * unselected;
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

void SimpleUI::Slider(std::string_view name,
                      const glm::vec4& region,
                      float* value, float min, float max)
{
    auto info = d_engine.Register(name, region);

    float x = info.quad.x;
    float y = info.quad.y;
    float width = info.quad.z;
    float height = info.quad.w;
    glm::vec4 leftColour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    glm::vec4 rightColour = d_theme.backgroundColour;
    float ratio = (*value - min) / (max - min);

    TextProperties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(leftColour, {x, y, ratio * width, height});
    cmd.AddQuad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    cmd.AddText(std::format("{}: {:.0f}", name, *value), info.quad, tp);

    if (info.sinceClicked > 0) {
        auto mouse = d_window->mouse_position();
        mouse.x = std::clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void SimpleUI::Dragger(std::string_view name,
                       const glm::vec4& region,
                       float* value, float speed)
{
    auto info = d_engine.Register(name, region);

    glm::vec4 colour = Interpolate(info, d_theme.baseColour, d_theme.hoveredColour, d_theme.clickedColour);
    
    TextProperties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.GetDrawCommand();
    cmd.AddQuad(colour, info.quad);
    cmd.AddText(std::format("{}:{:.2f}", name, *value), info.quad, tp);

    if (info.sinceClicked > 0) {
        *value += d_window->get_mouse_offset().x * speed;
    }    
}

void SimpleUI::Image(std::string_view name,
                     const texture* image,
                     const glm::vec2& position)
{
    glm::vec4 region{position.x, position.y, image->width(), image->height()};
    glm::vec4 copy = d_engine.ApplyOffset(region);

    DrawCommand cmd;
    cmd.vertices = {
        {{copy.x,          copy.y         }, {1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}},
        {{copy.x + copy.z, copy.y         }, {1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}},
        {{copy.x,          copy.y + copy.w}, {1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}},
        {{copy.x + copy.z, copy.y + copy.w}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}}
    };
    cmd.indices = {0, 1, 2, 2, 1, 3};
    cmd.texture = image;
    d_engine.SubmitDrawCommand(cmd);
}

}