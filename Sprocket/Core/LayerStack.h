#pragma once
#include "Core/Layer.h"
#include "Core/SceneData.h"

#include <vector>
#include <memory>

namespace Sprocket {

using LayerPtr = std::shared_ptr<Layer>;

class LayerStack
{
    std::vector<LayerPtr> d_layers;

public:
    LayerStack();

    void pushLayer(LayerPtr layer);
    LayerPtr popLayer();

    void update(SceneData* data);
        // Called on every tick. This goes through the stack
        // from the top and updates each layer. If a layer
        // returns True, none of the lower layers are updated.

    void draw();
        // Called on every tick. This does through the stack
        // from the bottom and draws each layer.

    void handleEvent(const Event& event);
        // Called on every event. This propagates the event to
        // every layer in the stack, starting at the top. If a layer
        // returns True, the event is not propagated to any lower
        // layers.
};

}