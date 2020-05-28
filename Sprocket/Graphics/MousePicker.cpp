#include "MousePicker.h"
#include "Log.h"

namespace Sprocket {
namespace MousePicker {

Maths::vec3 getRay(
    const Maths::vec2& mousePos,
    Window* window,
    Camera* camera,
    Lens* lens
)
{
    // Homogeneous Clip Space
    Maths::vec4 ray = {
        (2.0f * mousePos.x) / window->width() - 1,
        -((2.0f * mousePos.y) / window->height() - 1),
        -1.0f,
        1.0f
    };

    // Eye Space
    ray = Maths::inverse(lens->projection()) * ray;
    ray.z = -1.0f;
    ray.w = 0.0f;

    // World Space
    Maths::vec3 returnRay = Maths::inverse(camera->view()) * ray;
    Maths::normalise(returnRay);
    return returnRay;
}

}
}