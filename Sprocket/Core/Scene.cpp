#include "Scene.h"

namespace Sprocket {

void Scene::OnEvent(Event& event)
{
    for (size_t i = d_layers.size(); i != 0;) {
        --i;
        d_layers[i]->OnEvent(event);
    }
}

void Scene::OnUpdate(float dt)
{ 
    for (size_t i = 0; i != d_layers.size(); ++i) {
        d_layers[i]->OnUpdate(dt);
    }
}

}