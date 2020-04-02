#pragma once
#include "Layer.h"

#include <vector>
#include <memory>

namespace Sprocket {

using LayerPtr = std::shared_ptr<Layer>;

class Scene
{
    std::vector<LayerPtr> d_layers;

public:
    template <typename T, typename... Args>
    std::shared_ptr<T> add(Args&&... args);
        // Constructs and returns a shared pointer to a new layer.
        // Also adds to the internal stack.

    LayerPtr popLayer();

    void handleEvent(const Event& event);
        // Called on every event. This propagates the event to
        // every layer in the stack, starting at the top. If a layer
        // returns True, the event is not propagated to any lower
        // layers.

    void update();
        // Called on every tick. This goes through the stack
        // from the top and updates each layer. If a layer
        // returns True, none of the lower layers are updated.

    void draw();
        // Called on every tick. This does through the stack
        // from the bottom and draws each layer.
};

template <typename T, typename... Args>
std::shared_ptr<T> Scene::add(Args&&... args)
{
    auto layer = std::make_shared<T>(std::forward<Args>(args)...);
    d_layers.push_back(layer);
    return layer;
}

}