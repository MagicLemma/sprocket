#include "Scene.h"
#include "Components.h"
#include "Log.h"

namespace Sprocket {

Scene::Scene()
    : d_systems()
    , d_registry()
{
}

Scene::~Scene()
{
    SPKT_LOG_INFO("Deleting scene");
}

Entity Scene::NewEntity()
{
    auto e = d_registry.create();
    return Entity(&d_registry, e);
}

void Scene::AddSystem(std::shared_ptr<EntitySystem> system)
{
    d_systems.push_back(system);
}

void Scene::ClearSystems()
{
    d_systems.clear();
}

void Scene::OnStartup()
{
    for (auto system : d_systems) {
        system->OnStartup(*this);
    }
}

void Scene::OnUpdate(double dt)
{
    d_sinceLastSort += dt;

    if (d_sinceLastSort > 5.0) {
        d_registry.sort<ModelComponent>([](const auto& lhs, const auto& rhs) {
            return lhs.model < rhs.model || (lhs.model == rhs.model && lhs.texture < rhs.texture);
        });
        d_sinceLastSort -= 5.0f;
        SPKT_LOG_INFO("Sorted");
    }

    for (auto system : d_systems) {
        system->OnUpdate(*this, dt);
    }
}

void Scene::OnEvent(Event& event)
{
    for (auto system : d_systems) {
        system->OnEvent(*this, event);
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