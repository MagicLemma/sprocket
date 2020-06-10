#pragma once
#include "EntitySystem.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

namespace Sprocket {

class PlayerMovement : public EntitySystem
{
    float d_deltaTime;

    bool d_enabled = true;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

public:
    PlayerMovement();
    ~PlayerMovement() {}

    void UpdateEntity(float dt, Entity& entity) override;
    void UpdateSystem(float dt) override;

    void OnEvent(Event& event) override;

    void Enable(bool newEnabled);
};

}