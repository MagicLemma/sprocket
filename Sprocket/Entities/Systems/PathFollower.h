#pragma once
#include "EntitySystem.h"
#include "Scene.h"

namespace Sprocket {

class PathFollower : public EntitySystem
{
public:
    void OnUpdate(Scene& manager, double dt) override;
        // Called once per entity per frame and before the system updates.
};

}