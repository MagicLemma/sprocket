#include "Scene.h"
#include "Components.h"
#include "Loader.h"

namespace Sprocket {

Scene::Scene()
{
    d_registry.set_callback([&](ev::Event& event) {
        OnEvent(event);
    });
}

void Scene::Load(std::string_view file)
{
    Loader::Load(std::string(file), &Entities());
}

void Scene::OnUpdate(double dt)
{
    for (auto& system : d_systems) {
        system->on_update(d_registry, d_dispatcher, dt);
    }
}

void Scene::OnEvent(ev::Event& event)
{
    d_dispatcher.publish(event);
}

std::size_t Scene::Size() const
{
    return d_registry.size();
}

void Scene::Clear()
{
    d_registry.reset();
    d_lookup.clear();
    d_systems.clear();
}

}
