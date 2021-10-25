#pragma once
#include <sprocket/core/events.h>
#include <sprocket/core/window.h>

#include <array>

#include <glm/glm.hpp>

namespace spkt {

class input_store
{
public:
    static constexpr std::size_t NUM_KEYBOARD_KEYS = 512;
    static constexpr std::size_t NUM_MOUSE_BUTTONS = 5;

private:
    const spkt::window* const d_window; // Used to get mouse position and offset

    std::array<bool, NUM_KEYBOARD_KEYS> d_keyboard = {};
    std::array<bool, NUM_KEYBOARD_KEYS> d_keyboard_click = {};
    std::array<bool, NUM_KEYBOARD_KEYS> d_keyboard_unclick = {};

    std::array<bool, NUM_MOUSE_BUTTONS> d_mouse = {};
    std::array<bool, NUM_MOUSE_BUTTONS> d_mouse_click = {};
    std::array<bool, NUM_MOUSE_BUTTONS> d_mouse_unclick = {};

public:
    input_store(const spkt::window* window) : d_window(window) {}

    void on_event(spkt::event& event);

    // To be called at the end of each frame. Resets the per-frame data, such as
    // if a mouse or keyboard button was clicked/pressed this frame.
    void end_frame();

    glm::vec2 mouse_position() const { return d_window->mouse_position(); }
    glm::vec2 mouse_offset() const { return d_window->mouse_offset(); }
    glm::vec2 mouse_scrolled() const { return d_window->mouse_scrolled(); }

    bool is_mouse_down(int button) const { return d_mouse[button]; }
    bool is_mouse_clicked(int button) const { return d_mouse_click[button]; }
    bool is_mouse_unclicked(int button) const { return d_mouse_unclick[button]; }

    bool is_keyboard_down(int key) const { return d_keyboard[key]; }
    bool is_keyboard_clicked(int key) const { return d_keyboard_click[key]; }
    bool is_keyboard_unclicked(int key) const { return d_keyboard_unclick[key]; }

    float window_width() const { return (float)d_window->width(); }
    float window_height() const { return (float)d_window->height(); }
};
    
}