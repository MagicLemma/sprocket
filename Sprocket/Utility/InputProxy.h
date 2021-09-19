#pragma once
#include <Sprocket/Core/window.h>

#include <array>

#include <glm/glm.hpp>

namespace spkt {

class event;

class InputProxy
{
public:
    static constexpr std::size_t NUM_KEYS = 512;

private:
    std::array<bool, NUM_KEYS> d_keyboard;
    std::array<bool, 5> d_mouse;

public:
    InputProxy();

    void on_event(spkt::event& event);

    bool is_mouse_down(int button) const;
    bool is_keyboard_down(int key) const;
};

class input_store
{
public:
    static constexpr std::size_t NUM_KEYBOARD_KEYS = 512;
    static constexpr std::size_t NUM_MOUSE_BUTTONS = 5;

private:
    const spkt::window* const d_window; // Used to get mouse position and offset

    std::array<bool, NUM_KEYBOARD_KEYS> keyboard = {};
    std::array<bool, NUM_MOUSE_BUTTONS> mouse = {};
    std::array<bool, NUM_MOUSE_BUTTONS> mouse_click = {};
    std::array<bool, NUM_MOUSE_BUTTONS> mouse_unclick = {};

public:
    input_store(const spkt::window* window) : d_window(window) {}

    glm::vec2 mouse_position() const { return d_window->mouse_position(); }
    glm::vec2 mouse_offset() const { return d_window->mouse_offset(); }
};
    
}