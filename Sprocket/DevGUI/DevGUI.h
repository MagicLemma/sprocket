#pragma once
#include "Window.h"
#include "Event.h"

namespace Sprocket {

class DevGUI
{
    Window* d_window;

public:
    DevGUI(Window* window);
    ~DevGUI();

    void update(float dt);
    bool handleEvent(const Event& event);
};

}