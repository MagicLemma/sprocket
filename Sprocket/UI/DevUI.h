#pragma once
#include "Window.h"
#include "Shader.h"
#include "Event.h"
#include "Texture.h"
#include "StreamBuffer.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Sprocket {

class DevUI
// A class that wraps the setup and rendering of ImGui.
{
    Window* d_window;
    Shader  d_shader;
    std::unique_ptr<Texture> d_fontAtlas;

    StreamBuffer d_buffer;
        // Used to draw the render data created by ImGui.

    bool d_blockEvents;

public:
    DevUI(Window* window);

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void BlockEvents(bool val) { d_blockEvents = val; }

    void StartFrame();
    void EndFrame();
};

}