#pragma once
#include "Event.h"
#include "Maths.h"

#include <unordered_map>

namespace Sprocket {

class MouseProxy
{
    std::unordered_map<int, bool> d_pressedButtons;

    Maths::vec2 d_position;

    Maths::vec2 d_offsetSum;
        // Intermediate value to calulate the final offset

    Maths::vec2 d_offset;

public:
    MouseProxy();

    void update();  // Call at the start of every frame
    void handleEvent(Event& event);
    bool isButtonDown(int button) const;

    Maths::vec2 getMousePos() const { return d_position; }
    Maths::vec2 getMouseOffset() const { return d_offset; }
};

}