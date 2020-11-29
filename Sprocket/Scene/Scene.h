#pragma once
#include "Entity.h"
#include "EntitySystem.h"
#include "Event.h"
#include "Maths.h"

#include <memory>
#include <vector>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include <entt/entt.hpp>
#include <tsl/hopscotch_map.h>

namespace Sprocket {

struct Sun
{
    glm::vec3 direction;
    glm::vec3 colour;
    float       brightness;
};

struct Ambience
{
    glm::vec3 colour;
    float       brightness;
};
    
class Scene
{
public:
    using EntityCallback = std::function<void(Entity&)>;

    using TypeFunctionMap = tsl::hopscotch_map<
        std::type_index, std::vector<EntityCallback>
    >;

private:
    std::vector<std::shared_ptr<EntitySystem>> d_systems;

    TypeFunctionMap d_addFunctions;
    TypeFunctionMap d_updateFunctions;
    TypeFunctionMap d_removeFunctions;

    entt::registry d_registry;

    Sun d_sun;
    Ambience d_ambience;

    double d_sinceLastSort = 0.0f;

    template <typename T> void OnAddCB(entt::registry& r, entt::entity e);
    template <typename T> void OnUpdateCB(entt::registry& r, entt::entity e);
    template <typename T> void OnRemoveCB(entt::registry& r, entt::entity e);

public:
    Scene();
    ~Scene();
    
    Entity NewEntity();

    void AddSystem(std::shared_ptr<EntitySystem> system);
    void ClearSystems();

    void OnStartup();
    void OnUpdate(double dt, bool active);
    void OnEvent(Event& event);

    std::size_t Size() const;

    template <typename... Components> void Each(EntityCallback func);
        // Loops through all the entities that have the specified
        // components and calls the given lambda with each entity
        // as the argument.

    void All(EntityCallback func);

    template <typename T> void OnAdd(EntityCallback func);
    template <typename T> void OnUpdate(EntityCallback func);
    template <typename T> void OnRemove(EntityCallback func);
        // Register functions that will get called whenever
        // a component of type T is added/removed to/from an
        // Entity.

    void Clear();

    Sun& GetSun() { return d_sun; }
    const Sun& GetSun() const { return d_sun; }

    Ambience& GetAmbience() { return d_ambience; }
    const Ambience& GetAmbience() const { return d_ambience; }
};

template <typename... Components>
void Scene::Each(EntityCallback lambda)
{
    for (auto& entity : d_registry.view<Components...>()) {
        lambda(Entity(&d_registry, entity));   
    }
}

template <typename T>
void Scene::OnAdd(EntityCallback func)
{
    auto it = d_addFunctions.find(typeid(T));
    if (it == d_addFunctions.end()) {
        // Register the signal when we add the first function.
        d_registry.on_construct<T>()
            .connect<&Scene::OnAddCB<T>>(*this);
    }
    d_addFunctions[typeid(T)].push_back(func);
}

template <typename T>
void Scene::OnUpdate(EntityCallback func)
{
    auto it = d_updateFunctions.find(typeid(T));
    if (it == d_updateFunctions.end()) {
        // Register the signal when we add the first function.
        d_registry.on_construct<T>()
            .connect<&Scene::OnUpdateCB<T>>(*this);
    }
    d_updateFunctions[typeid(T)].push_back(func);
}

template <typename T>
void Scene::OnRemove(EntityCallback func)
{
    auto it = d_removeFunctions.find(typeid(T));
    if (it == d_removeFunctions.end()) {
        // Register the signal when we add the first function.
        d_registry.on_destroy<T>()
            .connect<&Scene::OnRemoveCB<T>>(*this);
    }
    d_removeFunctions[typeid(T)].push_back(func);
}

template <typename T>
void Scene::OnAddCB(entt::registry& r, entt::entity e)
{
    auto it = d_addFunctions.find(typeid(T));
    if (it != d_addFunctions.end()) {
        for (auto f : it->second) {
            f(Entity(&r, e));
        }
    }
}

template <typename T>
void Scene::OnUpdateCB(entt::registry& r, entt::entity e)
{
    auto it = d_updateFunctions.find(typeid(T));
    if (it != d_updateFunctions.end()) {
        for (auto f : it->second) {
            f(Entity(&r, e));
        }
    }
}

template <typename T>
void Scene::OnRemoveCB(entt::registry& r, entt::entity e)
{
    auto it = d_removeFunctions.find(typeid(T));
    if (it != d_removeFunctions.end()) {
        for (auto f : it->second) {
            f(Entity(&r, e));
        }
    }
}

}