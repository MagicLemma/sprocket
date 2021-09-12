#pragma once
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/Texture.h>

#include <imgui.h>
#include <ImGuizmo.h>

#include <memory>

namespace spkt {

class window;
class event;

void bind_imgui_vbo(std::uint32_t vbo);

using imgui_vertex_buffer = spkt::basic_buffer<ImDrawVert, buffer_usage::STREAM, bind_imgui_vbo>;
using imgui_index_buffer = spkt::index_buffer<ImDrawIdx, buffer_usage::STREAM>;

class DevUI
// A class that wraps the setup and rendering of ImGui.
{
    shader  d_shader;
    std::unique_ptr<texture> d_fontAtlas;

    spkt::imgui_vertex_buffer d_vtx_buffer;
    spkt::imgui_index_buffer  d_idx_buffer;

    bool d_blockEvents;

    DevUI(const DevUI&) = delete;
    DevUI& operator=(const DevUI&) = delete;

public:
    DevUI(window* window);

    void on_event(event& event);
    void on_update(double dt);

    void BlockEvents(bool val) { d_blockEvents = val; }

    void StartFrame();
    void EndFrame();
};

}