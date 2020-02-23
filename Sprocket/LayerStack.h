#pragma once
#include "Layer.h"
#include "Window.h"

#include <vector>
#include <memory>

namespace Sprocket {

using LayerPtr = std::shared_ptr<Layer>;

class LayerStack
{
    Window*               d_window;
    std::vector<LayerPtr> d_layers;
    bool                  d_showCursor;
        // This should be true if we want the mouse cursor to be
        // visible and false otherwise. We set this to true if
        // any active layer needs the mouse to be visible.

    void handleEvent(const Event& event);
        // Called on every event. This propagates the event to
        // every layer in the stack, starting at the top. If a layer
        // returns True, the event is not propagated to any lower
        // layers.

public:
    LayerStack(Window* window);

    void pushLayer(LayerPtr layer);
    LayerPtr popLayer();

    void update();
        // Called on every tick. This goes through the stack
        // from the top and updates each layer. If a layer
        // returns True, none of the lower layers are updated.

    void draw();
        // Called on every tick. This does through the stack
        // from the bottom and draws each layer.
};

}