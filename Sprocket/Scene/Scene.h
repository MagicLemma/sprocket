#pragma once
#include "ECS.h"
#include "EntitySystem.h"
#include "Event.h"
#include "Maths.h"

#include <memory>
#include <vector>

namespace Sprocket {
    
class Scene
{
    std::vector<std::shared_ptr<EntitySystem>> d_systems;

    ECS::Registry d_registry;

public:
    Scene();
    ~Scene();

    ECS::Registry* Reg() { return &d_registry; }

    template <typename T, typename... Args>
    T& Add(Args&&... args);

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
    auto system = std::make_shared<T>(std::forward<Args>(args)...);
    AddSystem(system);
    return *system;
}

}
