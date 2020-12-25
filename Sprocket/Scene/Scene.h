#pragma once
#include "ECS.h"
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
    std::vector<std::shared_ptr<EntitySystem>> d_systems;

    ECS::Registry d_registry;

    Sun d_sun;
    Ambience d_ambience;

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

    Sun& GetSun() { return d_sun; }
    const Sun& GetSun() const { return d_sun; }

    Ambience& GetAmbience() { return d_ambience; }
    const Ambience& GetAmbience() const { return d_ambience; }
};

}
