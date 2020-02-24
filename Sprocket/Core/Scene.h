#pragma once
#include "Core/SceneData.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"
#include "Events/Event.h"

#include <functional>

namespace Sprocket {

using EventHandler = std::function<void(const Event&, SceneData*)>;

class Scene
{
    SceneData d_data;
        // Global data for this scene. A pointer to this will be passed
        // to each layer on every tick of the game. This can also be
        // modified by the event system by implementing custom logic
        // via d_eventHandler.

    LayerStack d_layers;
        // The LayerStack for the scene. Layers are the main building
        // blocks for a scene and are responsible for creating everything
        // in the scene.

    EventHandler d_eventHandler;
        // Function that gets called on handleEvent, before sending the
        // evnt to the LayerStack. This is used to update the SceneData.

private:
    void handleEvent(const Event& event);
        // Called whenever an event happens. First calls the internal
        // event handler and then dispatches the event to the layer stack.

public:
    Scene(const SceneData&  data,
          const LayerStack& layers,
          EventHandler      eventHandler = [](const Event&, SceneData*){});

    void tick();
        // Should be called on every tick of the game loop. Updates the
        // layer stack and then draws it.
    
    void setEventHandler(EventHandler handler) { d_eventHandler = handler; }
        // Changes the event handler to the new one provided.
};

}