#pragma once
#include "Entity.h"
#include "EntitySystem.h"
#include "EntityRenderer.h"
#include "Event.h"

#include <vector>
#include <map>

namespace Sprocket {
    
class EntityManager
{
public:
    using EntityMap = std::map<std::size_t, std::shared_ptr<Entity>>;

private:
    const std::vector<EntitySystem*> d_systems;
        // All systems must be given at creation.

    EntityMap d_entities;

public:
    EntityManager(const std::vector<EntitySystem*> systems);

    void AddEntity(std::shared_ptr<Entity> entity);
    void OnUpdate(double dt);
    void OnEvent(Event& event);

    void Draw(EntityRenderer* renderer);

    const EntityMap& Entities() const { return d_entities; }
};

}