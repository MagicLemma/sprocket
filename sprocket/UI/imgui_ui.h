#pragma once
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/texture.h>

#include <imgui.h>
#include <ImGuizmo.h>

#include <memory>

namespace spkt {

class window;
class event;

void bind_imgui_vbo(std::uint32_t vbo);

using imgui_vertex_buffer = spkt::basic_buffer<ImDrawVert, buffer_usage::STREAM, bind_imgui_vbo>;
using imgui_index_buffer = spkt::index_buffer<ImDrawIdx, buffer_usage::STREAM>;

class imgui_ui
// A class that wraps the setup and rendering of ImGui.
{
    shader  d_shader;
    std::unique_ptr<texture> d_font_atlas;

    spkt::imgui_vertex_buffer d_vtx_buffer;
    spkt::imgui_index_buffer  d_idx_buffer;

    bool d_block_events;

    imgui_ui(const imgui_ui&) = delete;
    imgui_ui& operator=(const imgui_ui&) = delete;

public:
    imgui_ui(window* window);

    void on_event(event& event);
    void on_update(double dt);

    void block_events(bool val) { d_block_events = val; }

    void start_frame();
    void end_frame();
};

}