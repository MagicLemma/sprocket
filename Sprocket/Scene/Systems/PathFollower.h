#pragma once
#include "EntitySystem.h"
#include "Scene.h"

namespace spkt {

struct PathFollower : public EntitySystem
{
    void on_update(spkt::registry& registry, double dt) override;
};

}