#pragma once
#include "EntitySystem.h"
#include "Scene.h"

namespace Sprocket {

class PathFollower : public EntitySystem
{
public:
    void OnUpdate(Scene& scene, double dt, bool active) override;
        // Called once per entity per frame and before the system updates.
};

}