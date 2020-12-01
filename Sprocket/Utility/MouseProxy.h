#pragma once
#include "Event.h"
#include "Maths.h"

#include <array>

namespace Sprocket {

class MouseProxy
{
    std::array<bool, 5> d_pressed;

public:
    MouseProxy();

    void OnEvent(Event& event);
    bool IsButtonDown(int button) const;
};

}