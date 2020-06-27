#pragma once
#include "Event.h"
#include "Maths.h"

#include <array>

namespace Sprocket {

class MouseProxy
{
    // Stores the history of how the mouse buttons are pressed.
    std::array<bool, 5> d_pressedButtonsA;
        // Updated in the event loop.

    std::array<bool, 5> d_pressedButtonsB;
        // Copied from A at the start of OnUpdate. Stores the current state.

    std::array<bool, 5> d_pressedButtonsC;
        // Copied from B at the start of OnUpdate. Stores the previous state.

    Maths::vec2 d_position;

    Maths::vec2 d_offsetSum;
        // Intermediate value to calulate the final offset

    Maths::vec2 d_offset;

public:
    MouseProxy();

    void OnUpdate();  // Call at the start of every frame
    void OnEvent(Event& event);
    bool IsButtonDown(int button) const;

    Maths::vec2 GetMousePos() const { return d_position; }
    Maths::vec2 GetMouseOffset() const { return d_offset; }
};

}