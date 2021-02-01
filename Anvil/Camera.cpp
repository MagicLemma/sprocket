#include "Camera.h"

#include <glm/trigonometric.hpp>

namespace Sprocket {

Camera::Camera(Window* window, const glm::vec3& target)
    : d_window(window)
    , d_position()
    , d_target(target)
    , d_yaw(0.0f)
    , d_distance(8.0f)
    , d_moveSpeed(10.0f)
    , d_rotateSpeed(glm::half_pi<float>())
    , d_absVert(2.0f)
    , d_absMin(2.0f)
    , d_absMax(10.0f)
{
    d_input.ConsumeEventsFor(Keyboard::W);
    d_input.ConsumeEventsFor(Keyboard::A);
    d_input.ConsumeEventsFor(Keyboard::S);
    d_input.ConsumeEventsFor(Keyboard::D);
    d_input.ConsumeEventsFor(Keyboard::Q);
    d_input.ConsumeEventsFor(Keyboard::E);
}

void Camera::OnUpdate(double dt)
{
    float horizSpeed = d_rotateSpeed * dt;
    float moveSpeed = d_moveSpeed * dt;

    auto f = d_target - d_position;
    f.y = 0;
    f = glm::normalize(f);

    glm::vec3 up{0, 1, 0};
    glm::vec3 r = glm::cross(f, up);

    if (d_input.IsKeyboardDown(Keyboard::W)) {
        d_target += moveSpeed * f;
    }
    if (d_input.IsKeyboardDown(Keyboard::S)) {
        d_target -= moveSpeed * f;
    }
    if (d_input.IsKeyboardDown(Keyboard::D)) {
        d_target += moveSpeed * r;
    }
    if (d_input.IsKeyboardDown(Keyboard::A)) {
        d_target -= moveSpeed * r;
    }

    if (d_input.IsKeyboardDown(Keyboard::E)) {
        d_yaw -= horizSpeed;
    }
    if (d_input.IsKeyboardDown(Keyboard::Q)) {
        d_yaw += horizSpeed;
    }

    if (d_input.IsKeyboardDown(Keyboard::SPACE)) {
        d_position.y += d_moveSpeed * dt;
        d_target.y += d_moveSpeed * dt;
        d_absVert += d_moveSpeed * dt;
        d_absMin += d_moveSpeed * dt;
        d_absMax += d_moveSpeed * dt;
    }
    if (d_input.IsKeyboardDown(Keyboard::LSHIFT)) {
        d_position.y -= d_moveSpeed * dt;
        d_target.y -= d_moveSpeed * dt;
        d_absVert -= d_moveSpeed * dt;
        d_absMin -= d_moveSpeed * dt;
        d_absMax -= d_moveSpeed * dt;
    }

    d_position.x = d_target.x + d_distance * glm::cos(d_yaw);
    d_position.z = d_target.z + d_distance * glm::sin(d_yaw);

    if (d_position.y != d_absVert) {
        float d = d_absVert - d_position.y;
        d_position.y += 2 * d * dt;
    }
}

void Camera::OnEvent(Event& event)
{
    d_input.OnEvent(event);

    if (auto e = event.As<MouseScrolledEvent>()) {
        if (e->IsConsumed()) { return; }
        d_absVert -= e->YOffset();
        d_absVert = std::clamp(d_absVert, d_absMin, d_absMax);
        e->Consume();
    }
}

glm::mat4 Camera::Proj() const
{
    float aspectRatio = Viewport::CurrentAspectRatio();
    return glm::perspective(70.0f, aspectRatio, 0.1f, 1000.0f);
}

glm::mat4 Camera::View() const
{
    return glm::lookAt(d_position, d_target, {0.0, 1.0, 0.0});
}

}