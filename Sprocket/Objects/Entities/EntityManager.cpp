#include "EntityManager.h"

namespace Sprocket {

EntityManager::EntityManager(const std::vector<EntitySystem*> systems)
    : d_systems(systems)
{
}

void EntityManager::addEntity(std::shared_ptr<Entity> entity)
{
    d_entities.push_back(entity);
    for (auto system : d_systems) {
        system->registerEntity(*entity);
    }
}

void EntityManager::update(float dt)
{
    for (auto& entity : d_entities) {
        for (auto system : d_systems) {
            system->preUpdateEntity(*entity);
        }
    }

    for (auto system : d_systems) {
        system->updateSystem(dt);
    }

    for (auto& entity : d_entities) {
        for (auto system : d_systems) {
            system->postUpdateEntity(*entity);
        }
    }
}

}