#pragma once
#include "LuaEngine.h"
#include "EntitySystem.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

#include <unordered_map>

namespace Sprocket {

class ScriptRunner : public EntitySystem
{
    KeyboardProxy d_keyboard;
    MouseProxy    d_mouse;

    std::unordered_map<uint32_t, LuaEngine> d_engines;

public:
    ScriptRunner();

    void OnStartup(Scene& scene) override;
    void OnUpdate(Scene& scene, double dt, bool active) override;
    void OnEvent(Scene& scene, Event& event) override;
};

}