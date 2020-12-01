#pragma once
#include "LuaEngine.h"
#include "EntitySystem.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "Window.h"

#include <unordered_map>

namespace Sprocket {

class ScriptRunner : public EntitySystem
{
    Window*       d_window;
    KeyboardProxy d_keyboard;
    MouseProxy    d_mouse;

    std::unordered_map<uint32_t, LuaEngine> d_engines;

public:
    ScriptRunner(Window* window);

    void OnStartup(Scene& scene) override;
    void OnUpdate(Scene& scene, double dt) override;
    void OnEvent(Scene& scene, Event& event) override;
};

}