#include "Core/Scene.h"

namespace Sprocket {

Scene::Scene(const std::string& name,
             const LayerStack& layers,
             Window*           window)
    : d_name(name)
    , d_layers(layers)
    , d_window(window)
{
    d_window->registerCallback(d_name, [&](const Event& event) {
        handleEvent(event);
    });
}

Scene::~Scene()
{
    d_window->deregisterCallback(d_name);
}

void Scene::tick()
{
    d_layers.update(d_window);
    d_layers.draw(d_window);
}

void Scene::handleEvent(const Event& event)
{
    d_layers.handleEvent(d_window, event);
}

}