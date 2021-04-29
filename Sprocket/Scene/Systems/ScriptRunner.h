#pragma once
#include "LuaScript.h"
#include "ECS.h"
#include "EntitySystem.h"
#include "InputProxy.h"
#include "Window.h"

#include <unordered_map>
#include <apecs.hpp>

namespace Sprocket {

class ScriptRunner : public EntitySystem
{
    Window*    d_window;
    InputProxy d_input;

    std::unordered_map<spkt::entity, std::pair<lua::Script, bool>> d_engines;

    apx::generator<lua::Script&> active_scripts();

public:
    ScriptRunner(Window* window);

    void on_startup(spkt::registry& registry, ev::Dispatcher& dispatcher) override;
    void on_update(spkt::registry& registry, const ev::Dispatcher& dispatcher, double dt) override;
};

}