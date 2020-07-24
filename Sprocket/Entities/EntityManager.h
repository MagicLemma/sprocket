#pragma once
#include "Entity.h"
#include "EntitySystem.h"
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
public:
    using EntityCallback = std::function<void(Entity&)>;

    using TypeFunctionMap = std::unordered_map<
        std::type_index, std::vector<EntityCallback>
    >;

private:
    const std::vector<EntitySystem*> d_systems;
        // All systems must be given at creation.

    TypeFunctionMap d_addFunctions;
    TypeFunctionMap d_removeFunctions;

    entt::registry d_registry;

    template <typename T> void OnAddCB(entt::registry& r, entt::entity e);
    template <typename T> void OnRemoveCB(entt::registry& r, entt::entity e);

public:
    EntityManager(const std::vector<EntitySystem*> systems);
    
    Entity NewEntity();

    void OnStartup();
    void OnUpdate(double dt);
    void OnEvent(Event& event);

    std::size_t Size() const;

    template <typename... Components> void Each(EntityCallback func);
        // Loops through all the entities that have the specified
        // components and calls the given lambda with each entity
        // as the argument.

    template <typename T> void OnAdd(EntityCallback func);
    template <typename T> void OnRemove(EntityCallback func);
        // Register functions that will get called whenever
        // a component of type T is added/removed to/from an
        // Entity.
};

template <typename... Components>
void EntityManager::Each(EntityCallback lambda)
{
    for (auto& entity : d_registry.view<Components...>()) {
        lambda(Entity(&d_registry, entity));   
    }
}

template <typename T>
void EntityManager::OnAdd(EntityCallback func)
{
    auto it = d_addFunctions.find(typeid(T));
    if (it == d_addFunctions.end()) {
        // Register the signal when we add the first function.
        d_registry.on_construct<T>()
            .connect<&EntityManager::OnAddCB<T>>(*this);
    }
    d_addFunctions[typeid(T)].push_back(func);
}

template <typename T>
void EntityManager::OnRemove(EntityCallback func)
{
    auto it = d_removeFunctions.find(typeid(T));
    if (it == d_removeFunctions.end()) {
        // Register the signal when we add the first function.
        d_registry.on_destroy<T>()
            .connect<&EntityManager::OnRemoveCB<T>>(*this);
    }
    d_removeFunctions[typeid(T)].push_back(func);
}

template <typename T>
void EntityManager::OnAddCB(entt::registry& r, entt::entity e)
{
    auto it = d_addFunctions.find(typeid(T));
    if (it != d_addFunctions.end()) {
        for (auto f : it->second) {
            f(Entity(&r, e));
        }
    }
}

template <typename T>
void EntityManager::OnRemoveCB(entt::registry& r, entt::entity e)
{
    auto it = d_removeFunctions.find(typeid(T));
    if (it != d_removeFunctions.end()) {
        for (auto f : it->second) {
            f(Entity(&r, e));
        }
    }
}

}