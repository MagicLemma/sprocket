#pragma once
#include "EntitySystem.h"
#include "ECS.h"

namespace Sprocket {

struct CameraSystem : public EntitySystem
{
    void on_update(spkt::registry& registry, double dt) override;
};

}