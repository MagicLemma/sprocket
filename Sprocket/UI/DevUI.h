#pragma once
#include "Window.h"
#include "Maths.h"
#include "Shader.h"
#include "Event.h"
#include "Texture.h"
#include "Resources.h"
#include "StreamBuffer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include <memory>

namespace Sprocket {

class DevUI
{
    // SPROCKET OBJECTS
    Window* d_window;
    Shader  d_shader;
    Texture d_fontAtlas;

    StreamBuffer d_buffer;
        // Buffer used to store the render data created by ImGui
        // for rendering it.

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