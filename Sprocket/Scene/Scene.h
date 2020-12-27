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

    void AddSystem(std::shared_ptr<EntitySystem> system);
    void ClearSystems();

    void OnStartup();
    void OnUpdate(double dt);
    void OnEvent(Event& event);

    std::size_t Size() const;

    void Clear();
};

}
