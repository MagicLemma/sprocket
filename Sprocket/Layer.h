#pragma once
#include "Event.h"

namespace Sprocket {

class Layer
{
protected:

    enum class Status {
        INACTIVE = 0,
        PAUSED = 1,
        NORMAL = 2
    };

    Status d_status;

    bool   d_cursorEnabled;
        // True if the cursor needs to be visible and false otherwise.

private:
    Layer(Layer&&) = delete;
    Layer(const Layer&) = delete;
    Layer& operator=(const Layer&) = delete;
        // Layers are non-copyable and non-moveable.

protected:
    virtual bool handleEvent(const Event& event) = 0;
        // This function should contain all logic to handle Events being
        // sent to the layer.

    virtual void update(float tick) = 0;
        // This function should contain all logic to be run on every tick
        // of the application.

    virtual void draw() = 0;
        // This function should contain all logic to render the Layer to
        // the screen.

public:
    Layer(Status status, bool cursorVisible = true);

    // Virtual Interface
    bool callHandleEvent(const Event& event);
        // Called whenever an event happens. This function should return
        // True if the layer "consumed" the Event, and False otherwise.
        // Consuming the event means that the Event will not be propagated
        // down to lower layers. Layers will receive Events even if they
        // are inactive.

    void callUpdate(float tick);
        // Called in every tick of the game loop. The 'tick' argument is the
        // number of seconds elapsed since the start of the program.

    void callDraw();
        // Called in every tick of the game loop. Within a layer stack, these
        // are called in reverse order, starting at the bottom of the stack
        // and working upwards. If the layer is active, this function will
        // call "draw", otherwise it is not.

    // Helper Functions
    bool isActive() const;
    bool isCursorVisible() const;
};

}