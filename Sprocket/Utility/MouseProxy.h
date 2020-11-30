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

    bool d_consumeEvents = false;

public:
    MouseProxy();

    void OnUpdate();  // Call at the start of every frame
    void OnEvent(Event& event);
    bool IsButtonDown(int button) const;

    void ConsumeEvents(bool value) { d_consumeEvents = value; }

    bool IsButtonClicked(int button) const;
    bool IsButtonReleased(int button) const;
};

}