#pragma once
#include "Window.h"
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

namespace Sprocket {

class SimpleUI
{
    Window* d_window;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

public:
    SimpleUI(Window* window);

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    bool Button(const std::string& name,
                float x, float y,
                float width, float height);
};

}