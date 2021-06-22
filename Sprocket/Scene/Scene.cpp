#include "Scene.h"
#include "Components.h"
#include "Loader.h"

namespace Sprocket {

Scene::Scene()
{
    apx::meta::for_each(d_registry.tags, [&](auto&& tag) {
        using T = decltype(tag.type());
        d_registry.on_add<T>([&](apx::entity entity, const T&) {
            ev::Event event = ev::make_event<spkt::added<T>>(spkt::entity(d_registry, entity));
            OnEvent(event);
        });
        d_registry.on_remove<T>([&](apx::entity entity, const T&) {
            ev::Event event = ev::make_event<spkt::removed<T>>(spkt::entity(d_registry, entity));
            OnEvent(event);
        });
    });
}

Scene::~Scene()
{
    // We need to clear the registry before destruction of the dispatcher and
    // systems so that they are still valid when clearing the entities.
    d_registry.clear();
}

void Scene::Load(std::string_view file)
{
    Loader::Load(std::string(file), &Entities());
}

void Scene::OnUpdate(double dt)
{
    for (auto& system : d_systems) {
        system->on_update(d_registry, dt);
    }
}

void Scene::OnEvent(ev::Event& event)
{
    for (auto& system : d_systems) {
        system->on_event(d_registry, event);
    }
}

std::size_t Scene::Size() const
{
    return d_registry.size();
}

void Scene::Clear()
{
    d_registry.clear();
    d_lookup.clear();
    d_systems.clear();
}

}
