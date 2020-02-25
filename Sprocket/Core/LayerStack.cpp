#include "Core/LayerStack.h"

namespace Sprocket {

LayerStack::LayerStack()
    : d_layers()
{
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

void LayerStack::handleEvent(SceneData* data, const Event& event)
{
    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        if (d_layers[i]->handleEvent(data, event)) {
            break;
        }
    }
}

void LayerStack::update(SceneData* data)
{ 
    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        d_layers[i]->update(data);
    }
}

void LayerStack::draw(SceneData* data)
{
    for (size_t i = 0; i != d_layers.size(); ++i) {
        d_layers[i]->draw(data);
    }
}


}