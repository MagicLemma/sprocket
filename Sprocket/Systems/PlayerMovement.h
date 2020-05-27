#pragma once
#include "Window.h"
#include "EntitySystem.h"
#include "KeyboardProxy.h"

namespace Sprocket {

class PlayerMovement : public EntitySystem
{
    Window* d_window;
    float d_deltaTime;

    bool d_enabled = true;

    KeyboardProxy d_keyboard;

public:
    PlayerMovement(Window* window);
    ~PlayerMovement() {}

    void updateEntity(float dt, Entity& entity) override;
    void updateSystem(float dt) override;

    void handleEvent(Event& event) override;

    void registerEntity(const Entity& entity) override;
    void deregisterEntity(const Entity& entity) override;

    void addComponent(const Entity& entity, const Component& component) override {};
    void removeComponent(const Entity& entity, const Component& component) override {};

    void enable(bool newEnabled);
};

}