#pragma once
#include "Maths.h"
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class Camera
{
public:
    virtual Maths::mat4 view() const = 0;
        // By defaults returns the View matrix calculated from the position,
        // pitch, yaw and roll.

    virtual void update(Window* window, float timeDelta) = 0;
        // To be implemented, how this camera should update every tick.

    virtual void handleEvent(Window* window, const Event& event) {}
        // Optional override to handle events.
};

}