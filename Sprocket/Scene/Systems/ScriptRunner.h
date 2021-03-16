#pragma once
#include "LuaScript.h"
#include "ECS.h"
#include "EntitySystem.h"
#include "InputProxy.h"
#include "Window.h"

#include <unordered_map>

namespace Sprocket {

class ScriptRunner : public EntitySystem
{
    Window*    d_window;
    InputProxy d_input;

    std::unordered_map<ecs::Entity, std::pair<lua::Script, bool>> d_engines;

public:
    ScriptRunner(Window* window);

    void on_update(ecs::Registry& registry, double dt) override;
    void on_event(ecs::Registry& registry, ev::Event& event) override;
};

}