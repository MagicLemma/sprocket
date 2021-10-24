#pragma once
#include <Sprocket/UI/Font/font.h>
#include <Sprocket/UI/ui_engine.h>

#include <glm/glm.hpp>

#include <string>
#include <string_view>

namespace spkt {

class window;
class event;

struct simple_ui_theme
{
    glm::vec4 background_colour;
    glm::vec4 base_colour;
    glm::vec4 hovered_colour;
    glm::vec4 clicked_colour;
};

class simple_ui
{
    spkt::window* d_window;

    spkt::simple_ui_theme d_theme;

    spkt::ui_engine d_engine;
    spkt::font      d_font;

public:
    simple_ui(spkt::window* window);

    const spkt::simple_ui_theme& get_theme() const { return d_theme; }
    void set_theme(const spkt::simple_ui_theme& theme) { d_theme = theme; }

    spkt::font* get_font() { return &d_font; }
    
    void on_event(spkt::event& event);
    void on_update(double dt);

    void start_frame();
    void end_frame();

    void start_panel(std::string_view name, glm::vec4* region, panel_type type);
    void end_panel();

    void quad(const glm::vec4& colour, const glm::vec4& region);

    // TODO: Simplify the text API, it's a bit messy currently
    void text(
        std::string_view text,
        float size,
        const glm::vec4& region,
        const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0}
    );

    void text(
        std::string_view text,
        float size,
        const glm::vec2& position,
        const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0}
    );

    void text_modifiable(
        std::string_view name,
        const glm::vec4& region,
        std::string* text,
        const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0}
    );

    bool button(
        std::string_view name,
        const glm::vec4& region
    );

    bool checkbox(
        std::string_view name,
        const glm::vec4& region,
        bool* value
    );

    void slider(
        std::string_view name,
        const glm::vec4& region,
        float* value, float min, float max
    );

    void dragger(
        std::string_view name,
        const glm::vec4& region,
        float* value, float speed
    );

    void image(
        std::string_view name,
        const texture* image,
        const glm::vec2& position
    );

};

}