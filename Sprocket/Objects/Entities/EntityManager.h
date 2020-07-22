#pragma once
#include "Entity.h"
#include "EntitySystem.h"
#include "EntityRenderer.h"
#include "Event.h"

#include <vector>
#include <map>

#include <entt/entt.hpp>

namespace Sprocket {
    
class EntityManager
{
public:
    using EntityMap = std::map<entt::entity, std::shared_ptr<Entity>>;

private:
    const std::vector<EntitySystem*> d_systems;
        // All systems must be given at creation.

    entt::registry d_registry;

    EntityMap d_entities;

public:
    EntityManager(const std::vector<EntitySystem*> systems);

    std::shared_ptr<Entity> NewEntity();
    void AddEntity(std::shared_ptr<Entity> entity);
    void OnUpdate(double dt);
    void OnEvent(Event& event);

    void Draw(EntityRenderer* renderer);

    const EntityMap& Entities() const { return d_entities; }
};

}