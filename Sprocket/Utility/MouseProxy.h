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

    void OnUpdate();  // Call at the start of every frame
    void OnEvent(Event& event);
    bool IsButtonDown(int button) const;

    Maths::vec2 GetMousePos() const { return d_position; }
    Maths::vec2 GetMouseOffset() const { return d_offset; }
};

}