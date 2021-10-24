#include "ui_engine.h"

#include <sprocket/Core/events.h>
#include <sprocket/Core/input_codes.h>
#include <sprocket/Core/log.h>
#include <sprocket/Core/window.h>
#include <sprocket/Graphics/render_context.h>
#include <sprocket/UI/Font/font.h>
#include <sprocket/UI/Font/glyph.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <cassert>
#include <format>
#include <functional>
#include <ranges>
#include <vector>

namespace spkt {
namespace {

bool is_in_region(const glm::vec2& pos, const glm::vec4& quad)
{
    float x = quad.x;
    float y = quad.y;
    float width = quad.z;
    float height = quad.w;
    return x < pos.x && pos.x < x + width &&  y < pos.y && pos.y < y + height;
}

spkt::texture_data get_white_data()
{
    return {
        .width = 1,
        .height = 1,
        .bytes = {0xff, 0xff, 0xff, 0xff}
    };
}

}

void draw_command::add_quad(const glm::vec4& colour, const glm::vec4& quad)
{
    float x = quad.x;
    float y = quad.y;
    float width = quad.z;
    float height = quad.w;

    std::size_t index = vertices.size();
    vertices.push_back({{x,         y},          colour});
    vertices.push_back({{x + width, y},          colour});
    vertices.push_back({{x,         y + height}, colour});
    vertices.push_back({{x + width, y + height}, colour});

    indices.push_back(index + 0);
    indices.push_back(index + 1);
    indices.push_back(index + 2);
    indices.push_back(index + 2);
    indices.push_back(index + 1);
    indices.push_back(index + 3);
}

void draw_command::add_text(std::string_view text,
                            const glm::vec4& quad,
                            const text_properties& properties)
{
    if (text_font == nullptr) {
        log::error("Tried to add text to a draw command with no font!");
        return;
    }

    if (text.size() == 0) {
        return;
    }

    spkt::alignment alignment = properties.alignment;
    float size = properties.size;
    glm::vec4 colour = properties.colour;

    glm::vec2 pen{quad.x, quad.y};

    if (alignment == alignment::LEFT) {
        pen.x += 5.0f;
        pen.y += size;
    } else if (alignment == alignment::RIGHT) {
        pen.x += quad.z - 5.0f;
        pen.y += size;
    } else {
        float textWidth = text_font->text_width(text, size);
        spkt::glyph first = text_font->get_glyph(text.front(), size);
        pen.y += (quad.w - first.height) / 2.0f;
        pen.x += (quad.z - textWidth) / 2.0f;
        pen.x -= first.offset.x;
        pen.y += first.offset.y;
    }

    for (std::size_t i = 0; i != text.size(); ++i) {
        auto glyph = text_font->get_glyph(text[i], size);

        if (i > 0) {
            pen.x += text_font->get_kerning(text[i-1], text[i], size);
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

        std::uint32_t index = text_vertices.size();
        text_vertices.push_back({{xPos,         yPos},          colour, {x,     y    }});
        text_vertices.push_back({{xPos + width, yPos},          colour, {x + w, y    }});
        text_vertices.push_back({{xPos,         yPos + height}, colour, {x,     y + h}});
        text_vertices.push_back({{xPos + width, yPos + height}, colour, {x + w, y + h}});

        text_indices.push_back(index + 0);
        text_indices.push_back(index + 1);
        text_indices.push_back(index + 2);
        text_indices.push_back(index + 2);
        text_indices.push_back(index + 1);
        text_indices.push_back(index + 3);
    }
}

ui_engine::ui_engine(spkt::window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/SimpleUI.vert",
               "Resources/Shaders/SimpleUI.frag")
    , d_white(get_white_data())
{
}

glm::vec4 ui_engine::apply_offset(const glm::vec4& region)
{
    if (d_current_panel) {
        glm::vec4 quad = region;
        quad.x += d_current_panel->region.x;
        quad.y += d_current_panel->region.y;
        return quad;
    }
    return region;
}

widget_info ui_engine::register_region(std::string_view name, const glm::vec4& region)
{
    assert(d_current_panel);
    
    widget_info info;
    info.quad = apply_offset(region);
    
    std::string prefixedName = std::format("{}##{}", d_current_panel->name, name);
    std::size_t hash = std::hash<std::string>{}(prefixedName);
    d_current_panel->widget_regions.push_back({hash, info.quad});

    if (hash == d_clicked) {
        info.since_clicked = d_time - d_widget_times[hash].clicked_time;
        info.since_unlicked = 0.0;
    }
    else {
        info.since_clicked = 0.0;
        info.since_unlicked = d_time - d_widget_times[hash].unclicked_time;
    }

    if (hash == d_hovered) {
        info.since_hovered = d_time - d_widget_times[hash].hovered_time;
        info.since_unhovered = 0.0;
    }
    else {
        info.since_hovered = 0.0;
        info.since_unhovered = d_time - d_widget_times[hash].unhoveredTime;
    }

    if (hash == d_focused) {
        info.since_focused = d_time - d_widget_times[hash].focused_time;
        info.since_unfocused = 0.0;
        info.key_presses = d_key_presses;
    }
    else {
        info.since_focused = 0.0;
        info.since_unfocused = d_time - d_widget_times[hash].unfocused_time;
    }

    if (d_on_click == hash) { // Consume the onCLick
        d_on_click = 0;
        info.on_click = true;
    }

    if (d_on_hover == hash) { // Consume the onHover
        d_on_hover = 0;
        info.on_hover = true;
    }

    if (d_on_focus == hash) { // Consume the onFocus
        d_on_focus = 0;
        info.on_focus = true;
    }

    return info;
}

draw_command& ui_engine::get_draw_command()
{
    assert(d_current_panel);
    return d_current_panel->main_command;
}

void ui_engine::on_event(spkt::event& event)
{
    if (d_focused != 0 && !event.is_consumed()) {
        if (auto data = event.get_if<keyboard_typed_event>()) {
            d_key_presses.push_back(data->key);
            event.consume();
        }
        else if (auto data = event.get_if<keyboard_pressed_event>()) {
            if (data->key == Keyboard::BACKSPACE) {
                d_key_presses.push_back(Keyboard::BACKSPACE);
                event.consume();
            }
        }
        else if (auto data = event.get_if<keyboard_held_event>()) {
            if (data->key == Keyboard::BACKSPACE) {
                d_key_presses.push_back(Keyboard::BACKSPACE);
                event.consume();
            }
        }
    }

    if (auto data = event.get_if<mouse_pressed_event>()) {
        if (data->button == Mouse::LEFT) {
            d_mouse_clicked = true;
        }
        if (d_consume_mouse_events) { event.consume(); }
    }
    if (auto data = event.get_if<mouse_released_event>()) {
        if (data->button == Mouse::LEFT) {
            d_widget_times[d_clicked].unclicked_time = d_time;
            d_clicked = 0;
        }
    }
}

void ui_engine::on_update(double dt)
{
    d_time += dt;
}

void ui_engine::start_frame()
{
    d_panels.clear();
    d_current_panel = nullptr;
}

void ui_engine::mouse_click()
{
    bool foundClicked = false;
    const glm::vec2 mouse = d_window->mouse_position();

    std::size_t moveToFront = 0;

    for (auto panelHash : d_panel_order | std::views::reverse) {
        auto it = d_panels.find(panelHash);
        if (it == d_panels.end()) { continue; }
        const auto& panel = it->second;

        for (const auto& [hash, region] : panel.widget_regions | std::views::reverse) {
            auto clicked = is_in_region(mouse, region);

            if (!foundClicked && ((d_clicked == hash) || clicked)) {
                foundClicked = true;
                moveToFront = panelHash;
                if (d_clicked != hash) {
                    d_widget_times[d_clicked].unclicked_time = d_time;
                    d_widget_times[hash].clicked_time = d_time;
                    d_clicked = hash;
                    d_on_click = hash;

                    // The newly clicked widget is now the focus
                    d_widget_times[d_focused].unfocused_time = d_time;
                    d_widget_times[hash].focused_time = d_time;
                    d_focused = hash;
                    d_on_focus = hash;
                }
            }
        }
    }

    // Clicked on something other than the UI, so lose focus
    if (!foundClicked && d_focused > 0) {
        d_widget_times[d_focused].unfocused_time = d_time;
        d_focused = 0;
    }

    if (moveToFront > 0) {
        auto toMove = std::ranges::find(d_panel_order, moveToFront);
        d_panel_order.erase(toMove);
        d_panel_order.push_back(moveToFront);
    }
}

void ui_engine::mouse_hover()
{
    bool foundHovered = false;
    const glm::vec2 mouse = d_window->mouse_position();

    for (auto panelHash : d_panel_order | std::views::reverse) {
        auto it = d_panels.find(panelHash);
        if (it == d_panels.end()) { continue; }
        const auto& panel = it->second;

        for (const auto& [hash, region] : panel.widget_regions | std::views::reverse) {
            auto hovered = is_in_region(mouse, region);
            
            if (!foundHovered && hovered) {
                foundHovered = true;
                if (d_hovered != hash) {
                    d_widget_times[d_hovered].unhoveredTime = d_time;
                    d_widget_times[hash].hovered_time = d_time;
                    d_hovered = hash;
                    d_on_hover = hash;
                }
            }
        }
    }

    // Hovered over something other than the UI, so lose hover
    if (!foundHovered && d_hovered > 0) {
        d_widget_times[d_hovered].unhoveredTime = d_time;
        d_hovered = 0;
    }

    d_consume_mouse_events = foundHovered;
}

void ui_engine::end_frame()
{
    assert(!d_current_panel);

    if (d_mouse_clicked) {
        mouse_click();
    }
    d_mouse_clicked = false;
    mouse_hover();

    d_key_presses.clear();

    spkt::render_context rc;
    rc.alpha_blending(true);
    rc.face_culling(false);
    rc.depth_testing(false);

    const float w = (float)d_window->width();
    const float h = (float)d_window->height();

    // This transformation makes the top left of the screen (0, 0) and the bottom
    // right be (width, height). It flips the y-axis since OpenGL treats the bottom
    // left as (0, 0).
    const auto proj = glm::ortho(0.0f, w, h, 0.0f);
    d_shader.bind();
    d_shader.load("u_proj_matrix", proj);

    for (const auto& panelHash : d_panel_order) {
        const auto& panel = d_panels[panelHash];

        execute_command(panel.main_command);
        for (const auto& cmd : panel.extra_commands) {
            execute_command(cmd);
        }
    }
}

void ui_engine::start_panel(std::string_view name, glm::vec4* region, panel_type type)
{
    assert(!d_current_panel);
    const std::size_t hash = std::hash<std::string_view>{}(name);

    if (std::ranges::find(d_panel_order, hash) == d_panel_order.end()) {
        d_panel_order.push_back(hash);
    }
    
    auto& panel = d_panels[hash];
    panel.name = name;
    panel.hash = hash;
    panel.region = *region;
    d_current_panel = &panel;

    if (type == panel_type::CLICKABLE || type == panel_type::DRAGGABLE) {
        auto info = register_region(name, {0, 0, region->z, region->w});

        if (info.since_clicked > 0 && type == panel_type::DRAGGABLE) {
            region->x += d_window->mouse_offset().x;
            region->y += d_window->mouse_offset().y;
            d_current_panel->region = *region;
        }
    }
}

void ui_engine::end_panel()
{
    assert(d_current_panel);
    d_current_panel = nullptr;
}

void ui_engine::submit_draw_command(const draw_command& cmd)
{
    assert(d_current_panel);
    d_current_panel->extra_commands.emplace_back(cmd);
}

void ui_engine::execute_command(const draw_command& cmd)
{
    spkt::render_context rc;
    if (cmd.region.has_value()) {
        rc.set_scissor_window(*cmd.region);
    }

    if (cmd.texture) {
        cmd.texture->bind(0);
    } else {
        d_white.bind(0);
    }

    d_vertices.bind();
    d_indices.bind();

    d_vertices.set_data(cmd.vertices);
    d_indices.set_data(cmd.indices);
    glDrawElements(GL_TRIANGLES, (int)d_indices.size(), GL_UNSIGNED_INT, nullptr);

    if (cmd.text_font) {
        cmd.text_font->bind(0);
        d_shader.load("texture_channels", 1);
        d_vertices.set_data(cmd.text_vertices);
        d_indices.set_data(cmd.text_indices);
        glDrawElements(GL_TRIANGLES, (int)d_indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

}