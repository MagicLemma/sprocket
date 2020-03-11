#pragma once
#include "Core/Window.h"
#include "Events/Event.h"
#include "Utility/Maths.h"
#include "Camera.h"

namespace Sprocket {

class FirstPersonCamera : public Camera
{
    float d_sensitivity;
        // Mouse sensitivity

public:
    FirstPersonCamera();

    void update(Window* window, float timeDelta) override;
};

}