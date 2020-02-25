#include "Core/Scene.h"

namespace Sprocket {

Scene::Scene(const SceneData&  data,
             const LayerStack& layers,
             EventHandler      eventHandler)
    : d_data(data)
    , d_layers(layers)
    , d_eventHandler(eventHandler)
{
    data.window()->registerCallback(data.name(), [&](const Event& event) {
        handleEvent(event);
    });
}

Scene::~Scene()
{
    d_data.window()->deregisterCallback(d_data.name());
}

void Scene::tick()
{
    d_layers.update(&d_data);
    d_layers.draw();
}

void Scene::handleEvent(const Event& event)
{
    d_eventHandler(event, &d_data);
    d_layers.handleEvent(event);
}

}