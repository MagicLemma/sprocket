#include "Scene.h"
#include "Components.h"

namespace Sprocket {

Scene::Scene(const std::vector<EntitySystem*> systems)
    : d_systems(systems)
    , d_registry()
{
}

Entity Scene::NewEntity()
{
    auto e = d_registry.create();
    return Entity(&d_registry, e);
}

void Scene::OnStartup()
{
    for (auto& system : d_systems) {
        system->OnStartup(*this);
    }
}

void Scene::OnUpdate(double dt)
{
    for (auto system : d_systems) {
        system->OnUpdate(*this, dt);
    }
}

void Scene::OnEvent(Event& event)
{
    for (auto system : d_systems) {
        system->OnEvent(event);
    }
}

std::size_t Scene::Size() const
{
    return d_registry.alive();
}

void Scene::All(EntityCallback func)
{
    d_registry.each([&](auto entity) {
        func(Entity(&d_registry, entity));
    });
}

void Scene::Clear()
{
    d_registry.each([&](auto entity) {
        if (!d_registry.has<TemporaryComponent>(entity)) {
            d_registry.destroy(entity);
        }
    });
}

}