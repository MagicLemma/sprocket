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
    return d_registry.Size();
}

void Scene::All(EntityCallback func)
{
    for (ECS::Entity e : d_registry) {
        func(e);
    }
}

void Scene::Clear()
{
    for (ECS::Entity e : d_registry) {
        if (e.Has<TemporaryComponent>()) {
            e.Delete();
        }
    }
}

}
