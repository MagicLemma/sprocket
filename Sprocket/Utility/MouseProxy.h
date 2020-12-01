#pragma once
#include "Event.h"
#include "Maths.h"

#include <array>

namespace Sprocket {

class MouseProxy
{
    std::array<bool, 5> d_pressed;

    bool d_consumeEvents = false;

public:
    MouseProxy();

    void OnEvent(Event& event);
    bool IsButtonDown(int button) const;
    void ConsumeEvents(bool value);
};

}