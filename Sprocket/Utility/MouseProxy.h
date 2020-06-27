#pragma once
#include "Event.h"
#include "Maths.h"

#include <array>

namespace Sprocket {

class MouseProxy
{
    std::array<bool, 5> d_pressedTemp;
    std::array<bool, 5> d_pressedCurr;
    std::array<bool, 5> d_pressedPrev;
        // Current and prev store the state of the mouse on the current
        // frame and previous respectively. The temp is the one that the
        // event loop writes to. It is copied to current during OnUpdate.

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

    bool IsButtonClicked(int button) const;
    bool IsButtonReleased(int button) const;
};

}