#pragma once
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class UiComponent
{
public:
    virtual void update(Window* window) = 0;
    virtual void handleEvent(Window* window, const Event& event) = 0;
};

}