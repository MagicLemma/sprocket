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

    void UpdateEntity(float dt, Entity& entity) override;
    void UpdateSystem(float dt) override;

    void OnEvent(Event& event) override;

    void Enable(bool newEnabled);
};

}