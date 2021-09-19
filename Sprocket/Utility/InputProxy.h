#pragma once
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

struct input_store
{
    static constexpr std::size_t NUM_KEYS = 512;
    static constexpr std::size_t NUM_MOUSE_BUTTONS = 5;

    std::array<bool, NUM_KEYS> keyboard = {};
    std::array<bool, NUM_MOUSE_BUTTONS> mouse = {};
    std::array<bool, NUM_MOUSE_BUTTONS> mouse_click = {};
    std::array<bool, NUM_MOUSE_BUTTONS> mouse_unclick = {};
    glm::vec2 mouse_pos = {0.0, 0.0};
    glm::vec2 mouse_offset = {0.0, 0.0};
    glm::vec2 mouse_scrolled = {0.0, 0.0};
    float window_width = 1280.0f;
    float window_height = 720.0f;
    bool window_resized = false;
};
    
}