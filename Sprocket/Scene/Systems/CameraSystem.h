#pragma once
#include "EntitySystem.h"

namespace Sprocket {

class CameraSystem : public EntitySystem
{
public:
    void OnEvent(Scene& scene, Event& event) override;
};

}