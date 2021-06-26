#pragma once
#include "LuaScript.h"
#include "ECS.h"
#include "EntitySystem.h"

#include <unordered_map>
#include <apecs.hpp>

namespace Sprocket {

class ScriptRunner : public EntitySystem
{
    std::unordered_map<spkt::entity, std::pair<lua::Script, bool>> d_engines;

public:
    void on_event(spkt::registry& registry, ev::Event& event) override;
    void on_update(spkt::registry& registry, double dt) override;
};

}