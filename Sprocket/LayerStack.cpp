#include "LayerStack.h"

namespace Sprocket {

LayerStack::LayerStack(Window* window)
    : d_window(window)
    , d_layers()
    , d_showCursor(true)
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
        if (d_layers[i]->callHandleEvent(event)) {
            break;
        }
    }
}

void LayerStack::update()
{ 
    bool showCursor = false;

    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        d_layers[i]->callUpdate();
        if (d_layers[i]->isActive() && d_layers[i]->isCursorVisible()) {
            showCursor = true;
        }
    }

    if (d_showCursor != showCursor){
        d_window->setCursorVisibility(showCursor);
        d_showCursor = showCursor;
    }

}

void LayerStack::draw()
{
    for (size_t i = 0; i != d_layers.size(); ++i) {
        d_layers[i]->callDraw();
    }
}


}