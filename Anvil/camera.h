#pragma once
#include <Sprocket/Utility/InputProxy.h>

#include <glm/glm.hpp>

namespace spkt {
    class window;
    class event;
}

class Camera
{
    spkt::window*    d_window;
    spkt::InputProxy d_input;

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
    Camera(spkt::window* window, const glm::vec3& target);

    void on_update(double dt);
    void on_event(spkt::event& event);

    glm::mat4 Proj() const;
    glm::mat4 View() const;
};