#pragma once
#include "Window.h"
#include "Shader.h"
#include "Event.h"
#include "Texture.h"
#include "Resources.h"
#include "StreamBuffer.h"

#include <memory>

namespace Sprocket {

class DeveloperUI
{
    Window* d_window;
    Shader  d_shader;
    Texture d_fontAtlas;

    StreamBuffer d_buffer;
        // Buffer used to store the render data created by ImGui
        // for rendering it

public:
    DeveloperUI(Window* window);

    void handleEvent(Event& event);

    void startFrame(float dt);
    void endFrame();
};

}