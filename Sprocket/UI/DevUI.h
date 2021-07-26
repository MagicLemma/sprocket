#pragma once
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/Texture.h>

#include <imgui.h>
#include <ImGuizmo.h>

#include <memory>

namespace spkt {

class Window;
class event;

class DevUI
// A class that wraps the setup and rendering of ImGui.
{
    Window* d_window;
    shader  d_shader;
    std::unique_ptr<Texture> d_fontAtlas;

    std::uint32_t d_vao;
    std::uint32_t d_vertexBuffer;
    std::uint32_t d_indexBuffer;

    bool d_blockEvents;

    DevUI(const DevUI&) = delete;
    DevUI& operator=(const DevUI&) = delete;

public:
    DevUI(Window* window);

    void on_event(event& event);
    void on_update(double dt);

    void BlockEvents(bool val) { d_blockEvents = val; }

    void StartFrame();
    void EndFrame();
};

}