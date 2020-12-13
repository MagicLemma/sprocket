#pragma once
#include "Entity.h"
#include "EntitySystem.h"
#include "Event.h"
#include "Maths.h"
#include "ECS.h"

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
    float     brightness;
};

struct Ambience
{
    glm::vec3 colour;
    float     brightness;
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

    ECS::Registry d_registry;

    Sun d_sun;
    Ambience d_ambience;

    template <typename T> void OnAddCB(entt::registry& r, entt::entity e);
    template <typename T> void OnRemoveCB(entt::registry& r, entt::entity e);

public:
    Scene();
    ~Scene();
    
    Entity NewEntity();

    void AddSystem(std::shared_ptr<EntitySystem> system);
    void ClearSystems();

    void OnStartup();
    void OnUpdate(double dt);
    void OnEvent(Event& event);

    std::size_t Size() const;

    template <typename Component> void Each(EntityCallback func);
        // Loops through all the entities that have the specified
        // components and calls the given lambda with each entity
        // as the argument.

    void All(EntityCallback func);

    template <typename T> void OnAdd(EntityCallback func);
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

template <typename Component>
void Scene::Each(EntityCallback lambda)
{
    for (Entity e : d_registry.View<Component>()) {
        lambda(e);   
    }
}

template <typename T>
void Scene::OnAdd(EntityCallback func)
{
    d_registry.OnAdd<T>(func);
}

template <typename T>
void Scene::OnRemove(EntityCallback func)
{
    d_registry.OnRemove<T>(func);
}

}
