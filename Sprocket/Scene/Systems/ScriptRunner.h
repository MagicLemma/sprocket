#pragma once
#include "ECS.h"
#include "EntitySystem.h"

namespace spkt {

struct ScriptRunner : public EntitySystem
{
    void on_update(spkt::registry& registry, double dt) override;
};

}