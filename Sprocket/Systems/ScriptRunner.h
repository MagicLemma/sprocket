#pragma once
#include "EntitySystem.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

namespace Sprocket {

class ScriptRunner : public EntitySystem
{
    KeyboardProxy d_keyboard;
    MouseProxy    d_mouse;

public:
    ScriptRunner();

    void UpdateEntity(double dt, Entity& entity) override;

    void UpdateSystem(double dt) override;
    void OnEvent(Event& event) override;
};

}