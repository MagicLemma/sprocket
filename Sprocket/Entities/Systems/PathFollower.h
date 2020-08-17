#pragma once
#include "EntitySystem.h"
#include "EntityManager.h"

namespace Sprocket {

class PathFollower : public EntitySystem
{
public:
    void OnUpdate(Sprocket::EntityManager& manager, double dt) override;
        // Called once per entity per frame and before the system updates.
};

}