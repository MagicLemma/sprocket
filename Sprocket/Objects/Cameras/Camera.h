#pragma once
#include "Maths.h"
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class Camera
{
public:
    virtual ~Camera() {};

    virtual Maths::mat4 view() const = 0;

    virtual void update(Window* window, float timeDelta) = 0;
        // To be implemented, how this camera should update every tick.

    virtual bool handleEvent(Window* window, const Event& event) { return false; }
        // Optional override to handle events.
};

}