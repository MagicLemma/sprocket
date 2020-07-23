#include "EntityManager.h"

namespace Sprocket {

EntityManager::EntityManager(const std::vector<EntitySystem*> systems)
    : d_systems(systems)
    , d_registry()
{
}

Entity EntityManager::NewEntity()
{
    auto e = d_registry.create();
    return Entity(&d_registry, e);
}

void EntityManager::AddEntity(const Entity& entity)
{
    for (auto system : d_systems) {
        system->RegisterEntity(entity);
    }
}

void EntityManager::OnUpdate(double dt)
{
    for (auto system : d_systems) {
        system->OnUpdate(*this, dt);
    }

    Each<DeletableComponent>([&](Entity& entity) {
        if (!entity.Get<DeletableComponent>().alive) {
            for (auto system : d_systems) {
                system->DeregisterEntity(entity);
            }
            d_registry.destroy(entity.d_entity);
        }
    });
}

void EntityManager::OnEvent(Event& event)
{
    for (auto system : d_systems) {
        system->OnEvent(event);
    }
}

void EntityManager::Draw(EntityRenderer* renderer) {
    Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        renderer->Draw(entity);
    });
}

}