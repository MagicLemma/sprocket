#pragma once
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/buffer_element_types.h>
#include <Sprocket/Graphics/texture.h>

#include <glm/glm.hpp>

#include <chrono>
#include <deque>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace spkt {

class window;
class font;
class event;

enum class alignment
{
    LEFT,
    CENTRE,
    RIGHT
};

struct text_properties
{
    float     size      = 24.0f;
    alignment alignment = alignment::CENTRE;
    glm::vec4 colour    = {1.0, 1.0, 1.0, 1.0};
};

struct draw_command
{
    const texture*             texture = nullptr;
    std::vector<ui_vertex>     vertices;
    std::vector<std::uint32_t> indices;

    font*                      font = nullptr;
    std::vector<ui_vertex>     text_vertices;
    std::vector<std::uint32_t> text_indices;

    // If specified, we scissor test this region so nothing outside the region is rendered.
    std::optional<glm::vec4> region = {};

    // Helper for added a quad to the vertex and index buffer.
    void add_quad(const glm::vec4& colour, const glm::vec4& quad);

    // Helper for adding a string of test to the text vertex and index buffers. Uses the
    // font stored in the draw command.
    void add_text(std::string_view text,
                  const glm::vec4& quad,
                  const text_properties& properties = {});
};

// Describes how the encompassing region of the panel behaves. This has no impact on how
// the regions of widgets behave.
enum class panel_type
{
    // For unclickable panels, the background is completely non-interactable. Clicks will
    // not be absorbed and may affect things behind the panel.
    UNCLICKABLE,

    // For clickable panels, the background is also non-interactable, however clicks do
    // get absorbed and will bring the panel to the front.
    CLICKABLE,

    // For draggable panels, it is possible to click and drag the panels around the
    // viewport. This will also bring the panel to the front.
    DRAGGABLE
};

struct quad_data
{
    std::size_t hash;
    glm::vec4 region;
};

struct panel
{
    std::string name;
    std::size_t hash;
    glm::vec4   region;

    std::vector<quad_data> widget_regions;

    // Render data
    draw_command main_command;
    std::vector<draw_command> extra_commands;
};

struct widget_times
{
    double hovered_time   = 0.0;
    double unhoveredTime  = 0.0;
    double clicked_time   = 0.0;
    double unclicked_time = 0.0;
    double focused_time   = 0.0;
    double unfocused_time = 0.0;
};

struct widget_info
// When registering a new widget with the UIEngine, the callers gets
// this struct back. It contains, in seconds, the amount of time that
// the current widget has been (un)hovered and (un)clicked, among
// other pieces of info. The caller can then use this information to
// decide how the widget should be rendered.
{
    double since_hovered   = 0.0f;
    double since_unhovered = 0.0;

    double since_clicked  = 0.0;
    double since_unlicked = 0.0;

    double since_focused   = 0.0;
    double since_unfocused = 0.0;
    
    bool on_click = false;
    bool on_hover = false;
    bool on_focus = false;

    // The keys pressed since the last frame. This is guaranteed to be empty
    // if the current widget is not focused.
    std::vector<int> key_presses;

    // The region of the widget converted to screen space coords.
    glm::vec4 quad;
};

class ui_engine
{
    spkt::window* d_window;

    spkt::texture d_white;

    // Rendering code    
    spkt::shader d_shader;
    
    spkt::vertex_buffer<spkt::ui_vertex> d_vertices;
    spkt::index_buffer<std::uint32_t>    d_indices;
    
    // Panel info 
    std::unordered_map<std::size_t, panel> d_panels;
    panel* d_current_panel = nullptr;
    std::deque<std::size_t> d_panel_order;

    // Hashes of the currently hovered/clicked widgets.
    std::size_t d_clicked = 0;
    std::size_t d_hovered = 0;
    std::size_t d_focused = 0;

    // Hash -> time map keeping track of the last time each
    // widget was unselected. Used to calculate the unhovered
    // and unclicked times.
    std::unordered_map<std::size_t, widget_times> d_widget_times;

    // A steadily increasing timer used to set the unselected
    // times in the maps above.
    double d_time = 0.0;

    // Stores which widget has been clicked/hovered so that it can
    // be acted on next frame. These are consumed when retrieved.
    std::size_t d_on_click = 0;
    std::size_t d_on_hover = 0;
    std::size_t d_on_focus = 0;

    // A vector of key presses that happened since last frame. This will be
    // given to the currently focused widget.
    std::vector<int> d_key_presses;
    bool d_consume_mouse_events = false;
    bool d_mouse_clicked = false;

    void mouse_click();
    void mouse_hover();

    void execute_command(const draw_command& cmd);

public:
    ui_engine(spkt::window* window);

    // Registers a region that can respond to hovers and clicks. The (x, y)
    // position of the region is with respect to the position of the active panel.
    // If this widget will consume keyboard input, it needs to be stated here. Widgets
    // only consume keyboard events while in focus.
    widget_info register_region(std::string_view name, const glm::vec4& region);

    // Returns the region offsetted by the position of the current
    // panel if there is one, or region otherwise.
    glm::vec4 apply_offset(const glm::vec4& region);

    // Returns the current panels main draw command. Asserts if there is no current panel.
    draw_command& get_draw_command();

    // Submits an extra draw command for the current panel. Asserts if there is no current panel.
    void submit_draw_command(const draw_command& cmd);

    void on_event(spkt::event& event);
    void on_update(double dt);

    void start_frame();
    void end_frame();

    void start_panel(std::string_view name, glm::vec4* region, panel_type type);
    void end_panel();
};

}