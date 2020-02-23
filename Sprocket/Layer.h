#pragma once
#include "Event.h"

namespace Sprocket {

class Layer
{
protected:
    bool d_active;
        // True if the Layer is "active" and False otherwise.

    bool d_paused;
        // True is the Layer is "paused" and False otherwise.

    bool d_cursorEnabled;
        // True if the cursor needs to be visible and false otherwise.

private:
    Layer(Layer&&) = delete;
    Layer(const Layer&) = delete;
    Layer& operator=(const Layer&) = delete;
        // Layers are non-copyable and non-moveable.

public:
    Layer(bool isActive = true, bool cursorVisible = true, bool isPaused = false);

    // Virtual Interface
    virtual bool handleEvent(const Event& event) = 0;
        // Called whenever an event happens. This function should return
        // True if the layer "consumed" the Event, and False otherwise.
        // Consuming the event means that the Event will not be propagated
        // down to lower layers.

    virtual void update(float tick) = 0;
        // Called in every tick of the game loop. The 'tick' argument is the
        // number of seconds elapsed since the start of the program.

    virtual void draw() = 0;
        // Called in every tick of the game loop. Within a layer stack, these
        // are called in reverse order, starting at the bottom of the stack
        // and working upwards.

    // Helper Functions
    bool isActive() const;
    bool isCursorVisible() const;
};

}