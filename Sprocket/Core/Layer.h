#pragma once
#include "Core/SceneData.h"
#include "Events/Event.h"

namespace Sprocket {

class Layer
{
    float d_ticker;
        // Steadily increasing ticker. Bound to the framerate. 

protected:
    enum class Status {
        INACTIVE = 0,
        PAUSED = 1,
        NORMAL = 2
    };

    Status d_status;

private:
    Layer(Layer&&) = delete;
    Layer(const Layer&) = delete;
    Layer& operator=(const Layer&) = delete;
        // Layers are non-copyable and non-moveable.

    // Virtual Interface
    virtual bool handleEventImpl(const Event& event) = 0;
        // This function should contain all logic to handle Events being
        // sent to the layer.

    virtual void updateImpl(SceneData* data) = 0;
        // This function should contain all logic to be run on every tick
        // of the application.

    virtual void drawImpl() = 0;
        // This function should contain all logic to render the Layer to
        // the screen.

public:
    Layer(Status status, bool cursorVisible = true);

    bool handleEvent(const Event& event);
        // Called whenever an event happens. This function should return
        // True if the layer "consumed" the Event, and False otherwise.
        // Consuming the event means that the Event will not be propagated
        // down to lower layers. Layers will receive Events even if they
        // are inactive.

    void update(SceneData* data);
        // Called in every tick of the game loop.

    void draw();
        // Called in every tick of the game loop. Within a layer stack, these
        // are called in reverse order, starting at the bottom of the stack
        // and working upwards. It will only call "drawImpl" if the layer is
        // active.

    // Helper Functions
    bool isActive() const;

    float layerTicker() const;
        // Returns d_ticker, a steadily increasing value bound to the
        // framerate.
};

}