#include "LayerStack.h"

#include <chrono>

namespace Sprocket {

LayerStack::LayerStack(Window* window)
    : d_window(window)
    , d_layers()
    , d_isCursorVisible(true)
{
    window->registerCallback([&](const Event& event) {
        handleEvent(event);
    });
}

void LayerStack::pushLayer(LayerPtr layer)
{
    d_layers.push_back(layer);
}

LayerPtr LayerStack::popLayer()
{
    LayerPtr layer = d_layers.back();
    d_layers.pop_back();
    return layer;
}

void LayerStack::handleEvent(const Event& event)
{
    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        if (d_layers[i]->handleEvent(event)) {
            break;
        }
    }
}

void LayerStack::update()
{ 
    // First calculate the number of seconds since the start of the
    // program as a float to pass to each layer. This can be used for
    // updating positions in a way that does not depend on the framerate.
    using namespace std::chrono;
    static const steady_clock::time_point d_start = high_resolution_clock::now();
    float tick = duration<float>(high_resolution_clock::now() - d_start).count();

    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        d_layers[i]->update(tick);
    }

    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        if (d_layers[i]->isActive()) {
            bool isCursorVisible = d_layers[i]->isCursorVisible();
            if (d_isCursorVisible != isCursorVisible)
                {
                    d_window->setCursorVisibility(isCursorVisible);
                    d_isCursorVisible = isCursorVisible;
                }
            break;
        }
    }
}

void LayerStack::draw()
{
    for (size_t i = 0; i != d_layers.size(); ++i) {
        d_layers[i]->draw();
    }
}


}