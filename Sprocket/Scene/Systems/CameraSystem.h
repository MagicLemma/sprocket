#pragma once
#include "EntitySystem.h"
#include "ECS.h"

namespace Sprocket {

class CameraSystem : public EntitySystem
{
    float d_aspectRatio;

public:
    CameraSystem(float aspectRatio);
    void on_event(spkt::registry& registry, ev::Event& event) override;
};

}