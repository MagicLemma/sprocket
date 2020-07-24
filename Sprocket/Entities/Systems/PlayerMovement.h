#pragma once
#include "EntitySystem.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

namespace Sprocket {

class PlayerMovement : public EntitySystem
{
    bool d_enabled = true;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

public:
    PlayerMovement();
    ~PlayerMovement() {}

    void OnUpdate(EntityManager& manager, double dt) override;
    void OnEvent(Event& event) override;

    void Enable(bool newEnabled);
};

}