#include "Camera.h"

namespace Sprocket {

Camera::Camera(Window* window, const glm::vec3& target)
    : d_window(window)
    , d_position()
    , d_target(target)
    , d_yaw(0.0f)
    , d_distance(8.0f)
    , d_moveSpeed(10.0f)
    , d_rotateSpeed(90.0f)
    , d_absVert(2.0f)
    , d_absMin(2.0f)
    , d_absMax(10.0f)
{
    d_position = { d_distance, d_absMin, 0.0 };

    d_keyboard.ConsumeAll(false);
    d_keyboard.ConsumeEventsFor(Keyboard::W);
    d_keyboard.ConsumeEventsFor(Keyboard::A);
    d_keyboard.ConsumeEventsFor(Keyboard::S);
    d_keyboard.ConsumeEventsFor(Keyboard::D);
    d_keyboard.ConsumeEventsFor(Keyboard::Q);
    d_keyboard.ConsumeEventsFor(Keyboard::E);
}

void Camera::OnUpdate(double dt)
{
    d_mouse.OnUpdate();

    float horizSpeed = d_rotateSpeed * dt;
    float moveSpeed = d_moveSpeed * dt;

    auto f = d_target - d_position;
    f.y = 0;
    f = glm::normalize(f);

    glm::vec3 up{0, 1, 0};
    glm::vec3 r = glm::cross(f, up);

    if (d_keyboard.IsKeyDown(Keyboard::W)) {
        d_target += moveSpeed * f;
    }
    if (d_keyboard.IsKeyDown(Keyboard::S)) {
        d_target -= moveSpeed * f;
    }
    if (d_keyboard.IsKeyDown(Keyboard::D)) {
        d_target += moveSpeed * r;
    }
    if (d_keyboard.IsKeyDown(Keyboard::A)) {
        d_target -= moveSpeed * r;
    }

    if (d_keyboard.IsKeyDown(Keyboard::E)) {
        d_yaw -= horizSpeed;
    }
    if (d_keyboard.IsKeyDown(Keyboard::Q)) {
        d_yaw += horizSpeed;
    }

    if (d_keyboard.IsKeyDown(Keyboard::SPACE)) {
        d_position.y += d_moveSpeed * dt;
        d_target.y += d_moveSpeed * dt;
        d_absVert += d_moveSpeed * dt;
        d_absMin += d_moveSpeed * dt;
        d_absMax += d_moveSpeed * dt;
    }
    if (d_keyboard.IsKeyDown(Keyboard::LSHIFT)) {
        d_position.y -= d_moveSpeed * dt;
        d_target.y -= d_moveSpeed * dt;
        d_absVert -= d_moveSpeed * dt;
        d_absMin -= d_moveSpeed * dt;
        d_absMax -= d_moveSpeed * dt;
    }

    d_position.x = d_target.x + d_distance * Maths::Cosd(d_yaw);
    d_position.z = d_target.z + d_distance * Maths::Sind(d_yaw);

    if (d_position.y != d_absVert) {
        float d = d_absVert - d_position.y;
        d_position.y += 2 * d * dt;
    }
}

void Camera::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);

    if (auto e = event.As<MouseScrolledEvent>()) {
        if (e->IsConsumed()) { return; }
        d_absVert -= e->YOffset();
        d_absVert = std::clamp(d_absVert, d_absMin, d_absMax);
        e->Consume();
    }
}

glm::mat4 Camera::Proj() const
{
    return Maths::Perspective(d_window->AspectRatio(), 70.0f, 0.1f, 1000.0f);
}

glm::mat4 Camera::View() const
{
    return glm::lookAt(d_position, d_target, {0.0, 1.0, 0.0});
}

}