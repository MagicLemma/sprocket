#pragma once
#include "Window.h"
#include "Camera.h"
#include "Lens.h"
#include "Maths.h"

namespace Sprocket {
namespace MousePicker {

Maths::vec3 GetRay(
    const Maths::vec2& mousePos,
    Window* window,
    Camera* camera,
    Lens* lens
);

}
}