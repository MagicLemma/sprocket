#pragma once
#include "Window.h"
#include "EntitySystem.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

namespace Sprocket {

class PlayerMovement : public EntitySystem
{
    Window* d_window;
    float d_deltaTime;

    bool d_enabled = true;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

public:
    PlayerMovement(Window* window);
    ~PlayerMovement() {}

    void updateEntity(float dt, Entity& entity) override;
    void updateSystem(float dt) override;

    void handleEvent(Event& event) override;

    void enable(bool newEnabled);
};

}