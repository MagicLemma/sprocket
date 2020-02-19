#pragma once
#include "Window.h"

namespace Sprocket {

class Camera
{
    float   d_fov;
    float   d_nearPlane;
    float   d_farPlane;

    Window* d_window; // Non-owning

public:
    Camera(Window* window);

    float aspectRatio() const { return d_window->width() / d_window->height(); }
    float fov() const { return d_fov; }
    float nearPlane() const { return d_nearPlane; }
    float farPlane() const { return d_farPlane; }
};

}