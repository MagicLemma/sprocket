#include "Scene.h"

namespace Sprocket {

LayerPtr Scene::popLayer()
{
    LayerPtr layer = d_layers.back();
    d_layers.pop_back();
    return layer;
}

void Scene::handleEvent(const Event& event)
{
    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        if (d_layers[i]->handleEvent(event)) {
            break;
        }
    }
}

void Scene::update()
{ 
    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        d_layers[i]->update();
    }
}

void Scene::draw()
{
    for (size_t i = 0; i != d_layers.size(); ++i) {
        d_layers[i]->draw();
    }
}

}