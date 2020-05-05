#include "EntityManager.h"

namespace Sprocket {

EntityManager::EntityManager(const std::vector<EntitySystem*> systems)
    : d_systems(systems)
{
}

void EntityManager::addEntity(std::shared_ptr<Entity> entity)
{
    d_entities.insert(std::make_pair(entity->id(), entity));
    for (auto system : d_systems) {
        system->registerEntity(*entity);
    }
}

void EntityManager::update(float dt)
{
    for (auto& entity : d_entities) {
        for (auto system : d_systems) {
            system->preUpdateEntity(*(entity.second), dt);
        }
    }

    for (auto system : d_systems) {
        system->updateSystem(dt);
    }

    for (auto& entity : d_entities) {
        for (auto system : d_systems) {
            system->postUpdateEntity(*(entity.second), dt);
        }
    }
}

bool EntityManager::handleEvent(const Event& event)
{
    for (auto system : d_systems) {
        if (system->handleEvent(event)) {
            return true;
        }
    }
    return false;
}

bool EntityManager::deleteEntity(std::size_t id)
{
    auto entityIt = d_entities.find(id);
    if (entityIt == d_entities.end()) {
        return false;
    }

    for (auto system : d_systems) {
        system->deregisterEntity(*(entityIt->second));
    }

    d_entities.erase(id);
    return true;
}

bool EntityManager::deleteEntity(const Entity& entity)
{
    return deleteEntity(entity.id());
}

bool EntityManager::deleteEntity(Entity* entity)
{
    return deleteEntity(entity->id());
}

}