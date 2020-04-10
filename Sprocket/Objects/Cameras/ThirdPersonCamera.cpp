#include "ThirdPersonCamera.h"
#include "MouseEvent.h"
#include "Log.h"

#include <cmath>
#include <algorithm>

namespace Sprocket {

ThirdPersonCamera::ThirdPersonCamera()
    : Camera()
    , d_position({10.0f, 2.0f, 0.0f})
    , d_absoluteVerticalPosition(d_position.y)
    , d_target({0.0f, 0.0f, 0.0f})
    , d_distance(10.0f)
    , d_horiz(0.0f)
    , d_rotationSpeed(90.0f)
    , d_movementSpeed(10.0f)
{
}

Maths::mat4 ThirdPersonCamera::view() const
{
    return Maths::lookAt(d_position, d_target);
}

void ThirdPersonCamera::update(Window* window, float timeDelta)
{
    float horizSpeed = d_rotationSpeed * timeDelta;
    float moveSpeed = d_movementSpeed * timeDelta;

    Maths::vec3 forwards = d_target - d_position;
    forwards.y = 0;
    Maths::normalise(forwards);

    Maths::vec3 up = {0.0f, 1.0f, 0.0f};
    Maths::vec3 right = cross(forwards, up);
    
    if (window->isKeyDown(Keyboard::W)){
        d_target += moveSpeed * forwards;
    }
    if (window->isKeyDown(Keyboard::S)){
        d_target -= moveSpeed * forwards;
    }
    if (window->isKeyDown(Keyboard::D)){
        d_target += moveSpeed * right;
    }
    if (window->isKeyDown(Keyboard::A)){
        d_target -= moveSpeed * right;
    }

    if (window->isKeyDown(Keyboard::E)){
        d_horiz -= horizSpeed;
    }
    if (window->isKeyDown(Keyboard::Q)){
        d_horiz += horizSpeed;
    }

    d_position.x = d_target.x + d_distance * Maths::cosd(d_horiz);
    d_position.z = d_target.z + d_distance * Maths::sind(d_horiz);

    if (d_position.y != d_absoluteVerticalPosition) {
        float distance = d_absoluteVerticalPosition - d_position.y;
        d_position.y += distance * 0.1f;
    }
}

bool ThirdPersonCamera::handleEvent(Window* window, const Event& event)
{
    if (auto e = event.as<MouseScrolledEvent>()) {
        d_absoluteVerticalPosition -= e->yOffset();
        Maths::clamp(d_absoluteVerticalPosition, 2.0f, 10.0f);
    }
    return true;
}

}