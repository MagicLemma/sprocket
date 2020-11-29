#pragma once
#include "EntitySystem.h"

namespace Sprocket {

class AnimationSystem : public EntitySystem
{
public:
    void OnUpdate(Scene& scene, double dt, bool active) override;
};

}