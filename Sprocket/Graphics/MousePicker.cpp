#include "MousePicker.h"
#include "Log.h"

namespace Sprocket {
namespace MousePicker {

Maths::vec3 GetRay(
    const Maths::vec2& mousePos,
    Window* window,
    Camera* camera,
    Lens* lens
)
{
    // Homogeneous Clip Space
    Maths::vec4 ray = {
        (2.0f * mousePos.x) / window->Width() - 1,
        -((2.0f * mousePos.y) / window->Height() - 1),
        -1.0f,
        1.0f
    };

    // Eye Space
    ray = Maths::Inverse(lens->Projection()) * ray;
    ray.z = -1.0f;
    ray.w = 0.0f;

    // World Space
    Maths::vec3 returnRay = Maths::Inverse(camera->View()) * ray;
    Maths::Normalise(returnRay);
    return returnRay;
}

}
}