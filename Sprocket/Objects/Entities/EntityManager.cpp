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
    d_entities.emplace(entity.Id(), entity);
    for (auto system : d_systems) {
        system->RegisterEntity(entity);
    }
}

void EntityManager::OnUpdate(double dt)
{
    d_registry.each([](auto entity) {

    });
    auto it = d_entities.begin();
    while (it != d_entities.end()) {
        if (!Alive(it->second)) {
            for (auto system : d_systems) {
                system->DeregisterEntity(it->second);
            }
            it = d_entities.erase(it);
        }
        else {
            for (auto system : d_systems) {
                system->UpdateEntity(dt, it->second);
            }
            ++it;
        }
    }

    for (auto system : d_systems) {
        system->UpdateSystem(dt);
    }

    for (auto& [id, entity] : d_entities) {
        for (auto system : d_systems) {
            system->PostUpdateEntity(dt, entity);
        }
    }
}

void EntityManager::OnEvent(Event& event)
{
    for (auto system : d_systems) {
        system->OnEvent(event);
    }
}

void EntityManager::Draw(EntityRenderer* renderer) {
    for (auto& [id, entity]: d_entities) {
        renderer->Draw(entity);
    }
}

}