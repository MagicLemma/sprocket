#pragma once
#include "EntitySystem.h"

namespace Sprocket {

class CameraSystem : public EntitySystem
{
    float d_aspectRatio;

public:
    CameraSystem(float aspectRatio) : d_aspectRatio(aspectRatio) {}

    void OnUpdate(Scene& scene, double dt) override;
    void OnEvent(Event& event) override;
};

}