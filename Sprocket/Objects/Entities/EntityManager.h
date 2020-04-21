#pragma once
#include "Entity.h"
#include "EntitySystem.h"

#include <vector>

namespace Sprocket {
    
class EntityManager
{
    const std::vector<EntitySystem*> d_systems;
        // All systems must be given at creation.

    std::vector<std::shared_ptr<Entity>>        d_entities;

public:
    EntityManager(const std::vector<EntitySystem*> systems);

    void addEntity(std::shared_ptr<Entity> entity);
    void update(float dt);

    const std::vector<std::shared_ptr<Entity>>& entities() const { return d_entities; }
};

}