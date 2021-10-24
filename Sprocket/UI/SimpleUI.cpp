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
    const widget_info& info,
    const T& base,
    const T& hovered,
    const T& clicked)
{
    T ret = base;
    double interval = 0.1;
    
    if (info.since_hovered) {
        float r = std::min(info.since_hovered, interval) / (float)interval;
        ret = (1 - r) * ret + r * hovered;
    } else {
        float r = std::min(info.since_unhovered, interval) / (float)interval;
        ret = (1 - r) * hovered + r * ret;
    }

    if (info.since_clicked) {
        float r = std::min(info.since_clicked, interval) / (float)interval;
        ret = (1 - r) * ret + r * clicked;
    } else {
        float r = std::min(info.since_unlicked, interval) / (float)interval;
        ret = (1 - r) * clicked + r * ret;
    }

    return ret;
}

}

simple_ui::simple_ui(window* window)
    : d_window(window)
    , d_engine(window)
    , d_font("Resources/Fonts/Coolvetica.ttf")
{
}

void simple_ui::on_event(event& event)
{
    d_engine.on_event(event);
}

void simple_ui::on_update(double dt)
{
    d_engine.on_update(dt);
}

void simple_ui::start_frame()
{
    d_engine.start_frame();
}

void simple_ui::end_frame()
{
    d_engine.end_frame();
}

void simple_ui::start_panel(std::string_view name, glm::vec4* region, panel_type type)
{
    d_engine.start_panel(name, region, type);

    auto& cmd = d_engine.get_draw_command();
    cmd.text_font = &d_font;
    cmd.add_quad(d_theme.background_colour * 0.7f, *region);
}

void simple_ui::end_panel()
{
    d_engine.end_panel();
}

void simple_ui::quad(const glm::vec4& colour, const glm::vec4& quad)
{
    auto region = d_engine.apply_offset(quad);

    auto& cmd = d_engine.get_draw_command();
    cmd.add_quad(colour, region);
}

void simple_ui::text(
    std::string_view text,
    float size,
    const glm::vec4& quad,
    const glm::vec4& colour)
{
    auto region = d_engine.apply_offset(quad);

    text_properties tp;
    tp.colour = colour;
    tp.size = size;

    auto& cmd = d_engine.get_draw_command();
    cmd.add_text(text, region, tp);
}

void simple_ui::text(
    std::string_view text,
    float size,
    const glm::vec2& position,
    const glm::vec4& colour)
{
    auto region = d_engine.apply_offset({position.x, position.y, 0, 0});

    text_properties tp;
    tp.alignment = alignment::LEFT;
    tp.colour = colour;
    tp.size = size;

    auto& cmd = d_engine.get_draw_command();
    cmd.add_text(text, region, tp);
}

void simple_ui::text_modifiable(
    std::string_view name,
    const glm::vec4& region,
    std::string* text,
    const glm::vec4& colour)
{
    auto info = d_engine.register_region(name, region);

    draw_command cmd;
    cmd.region = info.quad;
    cmd.text_font = &d_font;

    for (int key : info.key_presses) {
        if (key == Keyboard::BACKSPACE) {
            if (text->size() > 0) {
                text->pop_back();
            }
        } else {
            text->push_back(static_cast<char>(key));
        }
    }

    auto boxColour = info.since_focused > 0 ? d_theme.hovered_colour : d_theme.base_colour;
    cmd.add_quad(boxColour, info.quad);

    std::string printText = *text;
    if (info.since_focused > 0 && modulo(info.since_focused, 1.0) < 0.5) {
        printText.push_back('|');
    }

    text_properties tp;
    tp.alignment = alignment::LEFT;
    tp.colour = colour;
    tp.size = 36.0f;

    cmd.add_text(printText, info.quad, tp);

    d_engine.submit_draw_command(cmd);
}

bool simple_ui::button(std::string_view name, const glm::vec4& region)
{
    auto info = d_engine.register_region(name, region);

    glm::vec4 hoveredRegion = info.quad;
    hoveredRegion.x -= 10.0f;
    hoveredRegion.z += 20.0f;

    glm::vec4 clickedRegion = info.quad;
    clickedRegion.x += 10.0f;
    clickedRegion.z -= 20.0f;

    glm::vec4 colour = Interpolate(info, d_theme.base_colour, d_theme.hovered_colour, d_theme.clicked_colour);
    glm::vec4 shape = Interpolate(info, info.quad, hoveredRegion, clickedRegion);

    text_properties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.get_draw_command();
    cmd.add_quad(colour, shape);
    cmd.add_text(name, info.quad, tp);

    return info.on_click;
}

bool simple_ui::checkbox(std::string_view name,
                        const glm::vec4& region,
                        bool* value)
{
    auto info = d_engine.register_region(name, region);

    auto unselected = Interpolate(info, d_theme.background_colour, d_theme.background_colour*1.1f, d_theme.clicked_colour);
    auto selected = Interpolate(info, d_theme.base_colour, d_theme.hovered_colour, d_theme.clicked_colour);;

    float r = std::min(info.since_clicked, 0.1) / 0.1f;

    glm::vec4 colour;
    if (*value) {
        colour = r * unselected + (1 - r) * selected;
    } else {
        colour = r * selected + (1 - r) * unselected;
    }

    if (info.on_click) {
        *value = !(*value);
    }

    text_properties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.get_draw_command();
    cmd.add_quad(colour, info.quad);
    cmd.add_text(name, info.quad, tp);

    return *value; 
}

void simple_ui::slider(std::string_view name,
                      const glm::vec4& region,
                      float* value, float min, float max)
{
    auto info = d_engine.register_region(name, region);

    float x = info.quad.x;
    float y = info.quad.y;
    float width = info.quad.z;
    float height = info.quad.w;
    glm::vec4 leftColour = Interpolate(info, d_theme.base_colour, d_theme.hovered_colour, d_theme.clicked_colour);
    glm::vec4 rightColour = d_theme.background_colour;
    float ratio = (*value - min) / (max - min);

    text_properties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.get_draw_command();
    cmd.add_quad(leftColour, {x, y, ratio * width, height});
    cmd.add_quad(rightColour, {x + ratio * width, y, (1 - ratio) * width, height});
    cmd.add_text(std::format("{}: {:.0f}", name, *value), info.quad, tp);

    if (info.since_clicked > 0) {
        auto mouse = d_window->mouse_position();
        mouse.x = std::clamp(mouse.x, x, x + width);
        float r = (mouse.x - x) / width;
        *value = (1 - r) * min + r * max;
    }    
}

void simple_ui::dragger(std::string_view name,
                       const glm::vec4& region,
                       float* value, float speed)
{
    auto info = d_engine.register_region(name, region);

    glm::vec4 colour = Interpolate(info, d_theme.base_colour, d_theme.hovered_colour, d_theme.clicked_colour);
    
    text_properties tp;
    tp.size = 36.0f;

    auto& cmd = d_engine.get_draw_command();
    cmd.add_quad(colour, info.quad);
    cmd.add_text(std::format("{}:{:.2f}", name, *value), info.quad, tp);

    if (info.since_clicked > 0) {
        *value += d_window->mouse_offset().x * speed;
    }    
}

void simple_ui::image(std::string_view name,
                     const texture* image,
                     const glm::vec2& position)
{
    glm::vec4 region{position.x, position.y, image->width(), image->height()};
    glm::vec4 copy = d_engine.apply_offset(region);

    draw_command cmd;
    cmd.vertices = {
        {{copy.x,          copy.y         }, {1.0, 1.0, 1.0, 1.0}, {0.0, 0.0}},
        {{copy.x + copy.z, copy.y         }, {1.0, 1.0, 1.0, 1.0}, {1.0, 0.0}},
        {{copy.x,          copy.y + copy.w}, {1.0, 1.0, 1.0, 1.0}, {0.0, 1.0}},
        {{copy.x + copy.z, copy.y + copy.w}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0}}
    };
    cmd.indices = {0, 1, 2, 2, 1, 3};
    cmd.texture = image;
    d_engine.submit_draw_command(cmd);
}

}