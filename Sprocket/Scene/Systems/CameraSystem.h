#pragma once
#include "EntitySystem.h"

namespace Sprocket {

class CameraSystem : public EntitySystem
{
    float d_aspectRatio;

public:
    CameraSystem(float aspectRatio);
    void OnEvent(ecs::Registry& registry, ev::Event& event) override;
};

}