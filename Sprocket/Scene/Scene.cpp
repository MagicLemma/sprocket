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
        system->OnUpdate(d_registry, dt);
    }
}

void Scene::OnEvent(ev::Event& event)
{
    for (auto& system : d_systems) {
        system->OnEvent(d_registry, event);
    }
}

std::size_t Scene::Size() const
{
    return d_registry.Size();
}

void Scene::Clear()
{
    d_registry.reset();
    d_lookup.clear();
    d_systems.clear();
}

}
