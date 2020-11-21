#pragma once
#include <Sprocket.h>

namespace Sprocket {

class Camera
{
    Window*       d_window;
    
    KeyboardProxy d_keyboard;
    MouseProxy    d_mouse;

    glm::vec3 d_position;
    glm::vec3 d_target;

    float d_yaw;
    float d_distance;

    float d_moveSpeed;
    float d_rotateSpeed;

    float d_absVert;
    float d_absMin;
    float d_absMax;

public:
    Camera(Window* window, const glm::vec3& target);

    void OnUpdate(double dt);
    void OnEvent(Event& event);

    glm::mat4 Proj() const;
    glm::mat4 View() const;
};

}