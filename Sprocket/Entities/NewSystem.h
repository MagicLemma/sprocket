#pragma once
#include "Event.h"

#include <entt/entt.hpp>

namespace Sprocket {

class NewSystem
{
public:
    virtual void OnAttach(entt::registry& registry) = 0;
        // Called when the system is first added to the Scene.

    virtual void OnUpdate(double dt, entt::registry& registry) = 0;
        // Called on every tick of the game loop.

    virtual void OnEvent(const Event& event, entt::registry& registry) = 0;
        // Called for every event received.
};

}