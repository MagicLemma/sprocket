#pragma once
#include "EntitySystem.h"
#include "Scene.h"

namespace Sprocket {

class PathFollower : public EntitySystem
{
public:
    void on_update(spkt::registry& registry, const ev::Dispatcher& dispatcher, double dt) override;
        // Called once per entity per frame and before the system updates.
};

}