#include "Camera.h"

namespace Sprocket {

Camera::Camera(Window* window)
    : d_window(window)
    , d_fov(70.0f)
    , d_nearPlane(0.1f)
    , d_farPlane(1000.0f)
{
}

}