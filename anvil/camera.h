#pragma once
#include <Sprocket/Utility/input_store.h>

#include <glm/glm.hpp>

namespace spkt {
class window;
class event;
}

namespace anvil {
    
class camera
{
    spkt::window*     d_window;
    spkt::input_store d_input;

    glm::vec3 d_position;
    glm::vec3 d_target;

    float d_yaw;
    float d_distance;

    float d_move_speed;
    float d_rotate_speed;

    float d_abs_vert;
    float d_abs_min;
    float d_abs_max;

public:
    camera(spkt::window* window, const glm::vec3& target);

    void on_update(double dt);
    void on_event(spkt::event& event);

    glm::mat4 proj() const;
    glm::mat4 view() const;
};

}