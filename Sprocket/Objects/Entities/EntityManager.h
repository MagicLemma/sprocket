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
    using EntityMap = std::map<entt::entity, Entity>;

private:
    const std::vector<EntitySystem*> d_systems;
        // All systems must be given at creation.

    entt::registry d_registry;

    EntityMap d_entities;

public:
    EntityManager(const std::vector<EntitySystem*> systems);

    Entity NewEntity();
    void AddEntity(const Entity& entity);
    void OnUpdate(double dt);
    void OnEvent(Event& event);

    void Draw(EntityRenderer* renderer);

    template <typename... Ts> void Each(std::function<void(Entity&)> lambda);

    std::size_t EntityCount() const { return d_registry.alive(); }
};

template <typename... Ts>
void EntityManager::Each(std::function<void(Entity&)> lambda)
{
    for (auto& entity : d_registry.view<Ts...>()) {
        lambda(Entity(&d_registry, entity));   
    }
}

}