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

    void updateSystem(float dt) override;
    void preUpdateEntity(Entity& entity, float dt) override;
    void postUpdateEntity(Entity& entity, float dt) override;

    bool handleEvent(const Event& event) override { return false; }

    void registerEntity(const Entity& entity) override;
    void deregisterEntity(const Entity& entity) override;

    void enable(bool newEnabled);
};

}