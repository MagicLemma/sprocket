#pragma once
#include "Core/LayerStack.h"
#include "Core/Window.h"
#include "Events/Event.h"

#include <functional>

namespace Sprocket {

using EventHandler = std::function<void(Window*, const Event&)>;

class Scene
{
    std::string d_name;
        // Name of the scene. This should be a unique identifier.

    LayerStack d_layers;
        // The LayerStack for the scene. Layers are the main building
        // blocks for a scene and are responsible for creating everything
        // in the scene.

    Window* d_window;
        // Non-owning pointer to the window that the scene wants to receive
        // events from.

private:
    void handleEvent(const Event& event);
        // Called whenever an event happens. First calls the internal
        // event handler and then dispatches the event to the layer stack.

public:
    Scene(const std::string& name, const LayerStack& layers, Window* window);
        // Additionally registers the Scene with the window speified in
        // the SceneData. Events from the window will be processed with
        // handleEvent.

    ~Scene();
        // Deregisters the Scene with the window that it is in.

    void tick();
        // Should be called on every tick of the game loop. Updates the
        // layer stack and then draws it.
};

}