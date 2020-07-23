#pragma once
#include "Entity.h"
#include "EntitySystem.h"
#include "EntityRenderer.h"
#include "Event.h"

#include <vector>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include <entt/entt.hpp>

namespace Sprocket {
    
class EntityManager
{
    const std::vector<EntitySystem*> d_systems;
        // All systems must be given at creation.

    std::unordered_map<
        std::type_index,
        std::vector<std::function<void(Entity&)>>
    > d_attachFunctions;

    entt::registry d_registry;

    template <typename T>
    void OnAddCallback(entt::registry& r, entt::entity e)
    {
        Entity entity(&r, e);
        auto it = d_attachFunctions.find(typeid(T));
        if (it != d_attachFunctions.end()) {
            for (auto f : it->second) {
                f(entity);
            }
        }
    }

public:
    EntityManager(const std::vector<EntitySystem*> systems);
    
    entt::registry& Reg() { return d_registry; }

    Entity NewEntity();
    void AddEntity(const Entity& entity);
    void OnUpdate(double dt);
    void OnEvent(Event& event);

    void Draw(EntityRenderer* renderer);

    template <typename... Components> void Each(std::function<void(Entity&)> lambda);
        // Loops through all the entities that have the specified components
        // and calls the given lambda with each entity as the argument.

    std::size_t EntityCount() const { return d_registry.alive(); }

    template <typename T> void OnAdd(std::function<void(Entity&)> func);
};

template <typename... Components>
void EntityManager::Each(std::function<void(Entity&)> lambda)
{
    for (auto& entity : d_registry.view<Components...>()) {
        lambda(Entity(&d_registry, entity));   
    }
}

template <typename T>
void EntityManager::OnAdd(std::function<void(Entity&)> func)
{
    auto it = d_attachFunctions.find(typeid(T));
    if (it == d_attachFunctions.end()) {
        // Register the signal when we add the first function.
        d_registry.on_construct<T>().connect<&EntityManager::OnAddCallback<T>>(*this);
    }
    d_attachFunctions[typeid(T)].push_back(func);
    SPKT_LOG_INFO("Num functions: {}", d_attachFunctions[typeid(T)].size());
}

template <typename T>
void OnAddCallback(entt::registry& r, entt::entity e)
{
    auto it = d_attachFunctions.find(typeid(T));
    if (it != d_attachFunctions.end()) {
        for (auto f : it->second) {
            f(Entity(&r, e));
        }
    }
}

}