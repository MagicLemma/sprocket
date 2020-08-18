#pragma once
#include "EntitySystem.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "PhysicsEngine.h"

namespace Sprocket {

class PlayerMovement : public EntitySystem
{
    PhysicsEngine* d_physicsEngine;

    bool d_enabled = true;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

public:
    PlayerMovement(PhysicsEngine* physicsEngine);
    ~PlayerMovement() {}

    void OnUpdate(Scene& scene, double dt) override;
    void OnEvent(Event& event) override;

    void Enable(bool newEnabled);
};

}