#pragma once
#include "ECS.h"
#include "EntitySystem.h"
#include "Event.h"
#include "Maths.h"

#include <memory>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace Sprocket {
    
class Scene
{
    std::unordered_map<std::type_index, std::size_t> d_lookup;
    std::vector<std::shared_ptr<EntitySystem>> d_systems;

    ECS::Registry d_registry;

public:
    Scene();
    ~Scene();

    ECS::Registry& Entities() { return d_registry; }

    template <typename T, typename... Args>
    T& Add(Args&&... args);

    template <typename T> bool Has();
    template <typename T> T& Get();

    void AddSystem(std::shared_ptr<EntitySystem> system);
    void ClearSystems();

    void OnUpdate(double dt);
    void OnEvent(Event& event);

    std::size_t Size() const;

    void Clear();
};

template <typename T, typename... Args>
T& Scene::Add(Args&&... args)
{
    assert(d_lookup.find(typeid(T)) == d_lookup.end());
    d_lookup[typeid(T)] = d_systems.size();
    auto system = std::make_shared<T>(std::forward<Args>(args)...);
    AddSystem(system);
    return *system;
}

template <typename T> bool Scene::Has()
{
    return d_lookup.find(typeid(T)) != d_lookup.end();
}

template <typename T> T& Scene::Get()
{
    auto it = d_lookup.find(typeid(T));
    assert(it != d_lookup.end());
    return *static_cast<T*>(d_systems[it->second].get());
}

}
