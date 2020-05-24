#include "Scene.h"

namespace Sprocket {

LayerPtr Scene::popLayer()
{
    LayerPtr layer = d_layers.back();
    d_layers.pop_back();
    return layer;
}

void Scene::handleEvent(Event& event)
{
    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        d_layers[i]->handleEvent(event);
    }
}

void Scene::update(float dt)
{ 
    for (size_t i = 0; i != d_layers.size(); ++i) {
        d_layers[i]->update(dt);
    }
}

}