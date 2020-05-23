#pragma once
#include "Event.h"
#include "Maths.h"

#include <unordered_map>

namespace Sprocket {

class MouseProxy
{
    std::unordered_map<int, bool> d_pressedButtons;

public:
    void handleEvent(Event& event);
    bool isButtonDown(int button) const;
};

}