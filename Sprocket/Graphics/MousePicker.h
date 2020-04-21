#pragma once
#include "Window.h"
#include "Camera.h"
#include "Lens.h"
#include "Maths.h"

namespace Sprocket {

class MousePicker
{
    Window* d_window;
    Camera* d_camera;
    Lens*   d_lens;
        // All non-owning references.

    Maths::vec3 d_currentRay;

    void calculateRay();

public:
    MousePicker(Window* window, Camera* camera, Lens* lens);

    Maths::vec3 currentRay() const { return d_currentRay; }

    void update();
};

}