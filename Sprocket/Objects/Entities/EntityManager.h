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
    void OnUpdate(float dt);
    void OnEvent(Event& event);

    void Draw(EntityRenderer* renderer);

    const EntityMap& Entities() const { return d_entities; }

    template <typename T>
    void NotifyComponentAdd(const Entity& entity)
        // Call after adding a component to this entity if the entity
        // if not already registered with the system.
    {
        const auto& c = entity.get<T>();
        for (auto& system : d_systems) {
            system->OnComponentAttach(entity, c);
        }
    }

    template <typename T>
    void NotifyComponentRemove(const Entity& entity)
        // Call BEFORE you remove the component from the entity. This does
        // not remove the component for you.
    {
        const auto& c = entity.get<T>();
        for (auto& system : d_systems) {
            system->OnComponentDetach(entity, c);
        }
    }
};

}