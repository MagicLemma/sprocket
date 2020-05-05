#pragma once
#include "Entity.h"
#include "EntitySystem.h"
#include "Event.h"

#include <vector>
#include <map>

namespace Sprocket {
    
class EntityManager
{
public:
    using Entities = std::map<std::size_t, std::shared_ptr<Entity>>;

    const std::vector<EntitySystem*> d_systems;
        // All systems must be given at creation.

    Entities d_entities;

public:
    EntityManager(const std::vector<EntitySystem*> systems);

    void addEntity(std::shared_ptr<Entity> entity);
    void update(float dt);
    bool handleEvent(const Event& event);

    bool deleteEntity(std::size_t id);
    bool deleteEntity(const Entity& entity);
    bool deleteEntity(Entity* entity);

    const Entities& entities() const { return d_entities; }
};

}