#include "Scene.h"

#include <Sprocket/Core/Events.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/Systems/input_system.h>

#include <ranges>

namespace spkt {

spkt::entity add_singleton(spkt::registry& registry)
{
    auto singleton = registry.create();
    registry.emplace<Runtime>(singleton);
    registry.emplace<Singleton>(singleton);
    registry.emplace<NameComponent>(singleton, "::RuntimeSingleton");
    registry.emplace<InputSingleton>(singleton);
    return singleton;
}

Scene::~Scene()
{
    registry.clear();
}

void Scene::on_update(double dt)
{
    spkt::input_system_begin(registry, dt);
    std::ranges::for_each(systems, [&](const system& sys) { sys(registry, dt); });
    spkt::input_system_end(registry, dt);
}

void Scene::on_event(spkt::event& event)
{
    spkt::input_system_on_event(registry, event);
}

}
