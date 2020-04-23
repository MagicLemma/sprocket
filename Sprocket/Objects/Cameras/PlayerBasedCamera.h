#pragma once
#include "Core/Window.h"
#include "Events/Event.h"
#include "Utility/Maths.h"
#include "Camera.h"
#include "Entity.h"

namespace Sprocket {

class PlayerBasedCamera : public Camera
{
    Entity* d_player;

public:
    PlayerBasedCamera(Entity* player);

    Maths::mat4 view() const override;
    void update(Window* window, float timeDelta) override;

    void setPlayer(Entity* player) { d_player = player; }
};

}