#pragma once
#include "Layer.h"

#include <vector>
#include <memory>

namespace Sprocket {

using LayerPtr = std::shared_ptr<Layer>;

class App
{
    std::vector<LayerPtr> d_layers;

public:
    // Constructs and returns a shared pointer to a new layer.
    // Also adds to the internal stack.
    template <typename T, typename... Args>
    std::shared_ptr<T> Add(Args&&... args);

    void OnEvent(Event& event);
    void OnUpdate(double dt);
    void OnRender();
};

template <typename T, typename... Args>
std::shared_ptr<T> App::Add(Args&&... args)
{
    auto layer = std::make_shared<T>(std::forward<Args>(args)...);
    d_layers.push_back(layer);
    return layer;
}

}