#pragma once
#include "Window.h"
#include "EntitySystem.h"

namespace Sprocket {

class PlayerMovement : public EntitySystem
{
    Window* d_window;
    float d_deltaTime;

    bool d_enabled = true;

public:
    PlayerMovement(Window* window);
    ~PlayerMovement() {}

    void updateEntity(float dt, Entity& entity) override;
    void updateSystem(float dt) override;

    void handleEvent(Event& event) override { return; }

    void registerEntity(const Entity& entity) override;
    void deregisterEntity(const Entity& entity) override;

    void enable(bool newEnabled);
};

}