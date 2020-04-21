#include "MousePicker.h"
#include "Log.h"

namespace Sprocket {

MousePicker::MousePicker(Window* window, Camera* camera, Lens* lens)
    : d_window(window)
    , d_camera(camera)
    , d_lens(lens)
{   
}

void MousePicker::calculateRay()
{
    // Pixel Space
    Maths::vec2 mousePos = d_window->getMousePos();

    // Normalised Screen Space
    mousePos = {
        (2.0f * mousePos.x) / d_window->width() - 1,
        -((2.0f * mousePos.y) / d_window->height() - 1)
    };

    // Homogeneous Clip Space
    Maths::vec4 ray = { mousePos.x, mousePos.y, -1.0f, 1.0f };

    // Eye Space
    ray = Maths::inverse(d_lens->projection()) * ray;
    ray.z = -1.0f;
    ray.w = 0.0f;

    // World Space
    ray = Maths::inverse(d_camera->view()) * ray;

    d_currentRay = {ray.x, ray.y, -1.0f};
    Maths::normalise(d_currentRay);
}

void MousePicker::update()
{
    calculateRay();
}

}