#pragma once
#include <Sprocket/Graphics/shader.h>
#include <Sprocket/Graphics/buffer.h>
#include <Sprocket/Graphics/buffer_element_types.h>
#include <Sprocket/Graphics/Texture.h>

#include <glm/glm.hpp>

#include <chrono>
#include <deque>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace spkt {

class Window;
class Font;
class event;

enum class Alignment
{
    LEFT,
    CENTRE,
    RIGHT
};

struct TextProperties
{
    float     size      = 24.0f;
    Alignment alignment = Alignment::CENTRE;
    glm::vec4 colour    = {1.0, 1.0, 1.0, 1.0};
};

struct DrawCommand
{
    const Texture*             texture = nullptr;
    std::vector<ui_vertex>  vertices;
    std::vector<std::uint32_t> indices;

    Font*                      font = nullptr;
    std::vector<ui_vertex>  textVertices;
    std::vector<std::uint32_t> textIndices;

    // If specified, we scissor test this region so nothing outside the region is rendered.
    std::optional<glm::vec4> region = {};

    // Helper for added a quad to the vertex and index buffer.
    void AddQuad(const glm::vec4& colour,
                 const glm::vec4& quad);

    // Helper for adding a string of test to the text vertex and index buffers. Uses the
    // font stored in the draw command.
    void AddText(std::string_view text,
                 const glm::vec4& quad,
                 const TextProperties& properties = {});
};

// Describes how the encompassing region of the panel behaves. This has no impact on how
// the regions of widgets behave.
enum class PanelType
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

struct QuadData
{
    std::size_t hash;
    glm::vec4 region;
};

struct Panel
{
    std::string name;
    std::size_t hash;
    glm::vec4 region;

    std::vector<QuadData> widgetRegions;

    // Render data
    DrawCommand mainCommand;
    std::vector<DrawCommand> extraCommands;
};

struct WidgetTimes
{
    double hoveredTime = 0.0;
    double unhoveredTime = 0.0;
    double clickedTime = 0.0;
    double unclickedTime = 0.0;
    double focusedTime = 0.0;
    double unfocusedTime = 0.0;
};

struct WidgetInfo
// When registering a new widget with the UIEngine, the callers gets
// this struct back. It contains, in seconds, the amount of time that
// the current widget has been (un)hovered and (un)clicked, among
// other pieces of info. The caller can then use this information to
// decide how the widget should be rendered.
{
    double sinceHovered = 0.0f;
    double sinceUnhovered = 0.0;

    double sinceClicked = 0.0;
    double sinceUnlicked = 0.0;

    double sinceFocused = 0.0;
    double sinceUnfocused = 0.0;
    
    bool onClick = false;
    bool onHover = false;
    bool onFocus = false;

    // The keys pressed since the last frame. This is guaranteed to be empty
    // if the current widget is not focused.
    std::vector<int> keyPresses;

    // The region of the widget converted to screen space coords.
    glm::vec4 quad;
};

class UIEngine
{
    spkt::Window* d_window;

    spkt::Texture d_white;

    // Rendering code    
    spkt::shader d_shader;
    
    spkt::vertex_buffer<spkt::ui_vertex> d_vertices;
    spkt::index_buffer<std::uint32_t>    d_indices;
    
    // Panel info 
    std::unordered_map<std::size_t, Panel> d_panels;
    Panel* d_currentPanel = nullptr;
    std::deque<std::size_t> d_panelOrder;

    // Hashes of the currently hovered/clicked widgets.
    std::size_t d_clicked = 0;
    std::size_t d_hovered = 0;
    std::size_t d_focused = 0;

    // Hash -> time map keeping track of the last time each
    // widget was unselected. Used to calculate the unhovered
    // and unclicked times.
    std::unordered_map<std::size_t, WidgetTimes> d_widgetTimes;

    // A steadily increasing timer used to set the unselected
    // times in the maps above.
    double d_time = 0.0;

    // Stores which widget has been clicked/hovered so that it can
    // be acted on next frame. These are consumed when retrieved.
    std::size_t d_onClick = 0;
    std::size_t d_onHover = 0;
    std::size_t d_onFocus = 0;

    // A vector of key presses that happened since last frame. This will be
    // given to the currently focused widget.
    std::vector<int> d_keyPresses;
    bool d_consumeMouseEvents = false;
    bool d_mouseClicked = false;

    void MouseClick();
    void MouseHover();

    void ExecuteCommand(const DrawCommand& cmd);

public:
    UIEngine(spkt::Window* window);

    // Registers a region that can respond to hovers and clicks. The (x, y)
    // position of the region is with respect to the position of the active panel.
    // If this widget will consume keyboard input, it needs to be stated here. Widgets
    // only consume keyboard events while in focus.
    WidgetInfo Register(std::string_view name, const glm::vec4& region);

    // Returns the region offsetted by the position of the current
    // panel if there is one, or region otherwise.
    glm::vec4 ApplyOffset(const glm::vec4& region);

    // Returns the current panels main draw command. Asserts if there is no current panel.
    DrawCommand& GetDrawCommand();

    // Submits an extra draw command for the current panel. Asserts if there is no current panel.
    void SubmitDrawCommand(const DrawCommand& cmd);

    void on_event(spkt::event& event);
    void on_update(double dt);

    void StartFrame();
    void EndFrame();

    void StartPanel(std::string_view name, glm::vec4* region, PanelType type);
    void EndPanel();
};

}