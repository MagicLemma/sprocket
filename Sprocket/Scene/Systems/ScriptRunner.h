#pragma once
#include "LuaScript.h"
#include "ECS.h"
#include "EntitySystem.h"
#include "InputProxy.h"
#include "Window.h"

#include <unordered_map>
#include <cppcoro/generator.hpp>

namespace Sprocket {

class ScriptRunner : public EntitySystem
{
    Window*    d_window;
    InputProxy d_input;

    std::unordered_map<ecs::Entity, std::pair<lua::Script, bool>> d_engines;

    cppcoro::generator<lua::Script&> active_scripts();

public:
    ScriptRunner(Window* window);

    void on_startup(ecs::Registry& registry, ev::Dispatcher& dispatcher) override;
    void on_update(ecs::Registry& registry, const ev::Dispatcher& dispatcher, double dt) override;
};

}