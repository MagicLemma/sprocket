#pragma once
#include "CoreSystems.h"

namespace Sprocket {

class Event;

class Layer
{
protected:  
    CoreSystems d_core;

private:
    Layer(Layer&&) = delete;
    Layer(const Layer&) = delete;
    Layer& operator=(const Layer&) = delete;
        // Layers are non-copyable and non-moveable.

public:
    Layer(const CoreSystems& core);
    virtual ~Layer() {}

    virtual void OnEvent(Event& event) = 0;
        // Called whenever an event happens. This function should return
        // True if the layer "consumed" the Event, and False otherwise.
        // Consuming the event means that the Event will not be propagated
        // down to lower layers. Layers will receive Events even if they
        // are inactive.

    virtual void OnUpdate(float dt) = 0;
        // Called in every tick of the game loop. Within a layer stack, these
        // are called in reverse order, starting at the bottom of the stack
        // and working upwards.
};

}