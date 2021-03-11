#pragma once
#include <array>
#include <unordered_map>
#include <unordered_set>

namespace Sprocket {
namespace ev { class Event; }

class InputProxy
{
public:
    static constexpr std::size_t NUM_KEYS = 512;

private:
    std::array<bool, NUM_KEYS> d_keyboard;
    std::array<bool, 5> d_mouse;

public:
    InputProxy();

    void on_event(ev::Event& event);

    bool is_mouse_down(int button) const;
    bool is_keyboard_down(int key) const;
};
    
}