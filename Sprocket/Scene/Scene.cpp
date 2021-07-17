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
    d_registry.clear();
}

void Scene::add(const system_t& system)
{
    d_systems.push_back(system);
}

void Scene::on_update(double dt)
{
    spkt::input_system_begin(d_registry, dt);
    std::ranges::for_each(d_systems, [&](auto&& system) { system(d_registry, dt); });
    spkt::input_system_end(d_registry, dt);
}

void Scene::on_event(ev::Event& event)
{
    spkt::input_system_on_event(d_registry, event);
}

}
