#include "EntityManager.h"

namespace Sprocket {

std::shared_ptr<Entity> EntityManager::addEntity()
{
    auto entity = std::make_shared<Entity>();
    d_entities.push_back(entity);
    return entity;
}

void EntityManager::update()
{
    for (auto& entity : d_entities) {
        entity->update();
    }
}

bool EntityManager::handleEvent(const Event& event)
{
    for (auto& entity : d_entities) {
        if (entity->handleEvent(event)) {
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<Entity>> EntityManager::entities()
{
    return d_entities;
}

}