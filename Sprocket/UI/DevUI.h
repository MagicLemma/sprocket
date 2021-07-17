#pragma once
#include <Sprocket/Graphics/Shader.h>
#include <Sprocket/Graphics/StreamBuffer.h>
#include <Sprocket/Graphics/Texture.h>

#include <imgui.h>
#include <ImGuizmo.h>

#include <memory>

namespace spkt {

class Window;
namespace ev { class Event; }

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

    void on_event(ev::Event& event);
    void on_update(double dt);

    void BlockEvents(bool val) { d_blockEvents = val; }

    void StartFrame();
    void EndFrame();
};

}