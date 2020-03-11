#pragma once
#include "Maths.h"
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class Lens
// Class for representing the projection matrix for rendering.
{
public:
    virtual Maths::mat4 projection() const = 0;
        // Returns a projection matrix.

    virtual void handleEvent(Window* window, const Event& event) {}
        // This function may be overriden if the Lens needs to
        // respond to events.
};

}