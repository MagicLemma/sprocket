#pragma once
#include "Entity.h"
#include "Event.h"

namespace Sprocket {

class Listener
{
    Entity* d_camera;

public:
    Listener(Entity* camera = nullptr);

    void OnUpdate();

    void SetCamera(Entity* camera) { d_camera = camera; }
};


}