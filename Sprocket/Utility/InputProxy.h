#pragma once
#include "Event.h"

#include <array>
#include <unordered_map>
#include <unordered_set>

namespace Sprocket {

class InputProxy
{
    // Mouse
    std::array<bool, 5> d_buttons;

    // Keyboard
    std::unordered_map<int, bool> d_keys;
    std::unordered_set<int> d_consumedKeys;

public:
    void OnEvent(Event& event);

    bool IsMouseDown(int button) const;
    bool IsKeyboardDown(int key) const;

    void ConsumeEventsFor(int key);
};
    
}