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
    auto it = d_entities.begin();
    while (it != d_entities.end()) {
        if (!it->second->alive()) {
            for (auto system : d_systems) {
                system->deregisterEntity(*(it->second));
            }
            it = d_entities.erase(it);
        }
        else {
            for (auto system : d_systems) {
                system->updateEntity(dt, *(it->second));
            }
            ++it;
        }
    }

    for (auto system : d_systems) {
        system->updateSystem(dt);
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

}