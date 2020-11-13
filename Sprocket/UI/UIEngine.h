#pragma once
#include "Window.h"
#include "Shader.h"
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "StreamBuffer.h"
#include "Font.h"
#include "Types.h"

#include <vector>
#include <unordered_map>
#include <optional>
#include <chrono>
#include <deque>

namespace Sprocket {

enum class Alignment
{
    LEFT,
    CENTRE,
    RIGHT
};

struct TextProperties
{
    float       size      = 24.0f;
    Alignment   alignment = Alignment::CENTRE;
    Maths::vec4 colour    = {1.0, 1.0, 1.0, 1.0};
};

struct BufferVertex
{
    Maths::vec2 position;
    Maths::vec4 colour;
    Maths::vec2 textureCoords = {0.0, 0.0};
};

struct QuadData
{
    std::size_t hash;
    Maths::vec4 region;
};

struct DrawCommand
{
    Texture*                  texture = nullptr;
    std::vector<BufferVertex> vertices;
    std::vector<u32>          indices;

    Font*                     font = nullptr;
    std::vector<BufferVertex> textVertices;
    std::vector<u32>          textIndices;

    // If specified, we scissor test this region so nothing outside the region is rendered.
    std::optional<Maths::vec4> region = {};

    void AddQuad(const Maths::vec4& colour,
                 const Maths::vec4& quad);

    void AddText(const std::string& text,
                 const Maths::vec4& quad,
                 const TextProperties& properties = {});
};

struct Panel
{
    std::string name;
    std::size_t hash;
    Maths::vec4 region;

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
    Maths::vec4 quad;
};

class UIEngine
{
    Window* d_window;

    KeyboardProxy* d_keyboard;
    MouseProxy* d_mouse;

    Texture d_white;

    // Rendering code    
    Shader d_shader;
    StreamBuffer d_buffer;
    
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

    void ExecuteCommand(const DrawCommand& cmd);

public:
    UIEngine(Window* window, KeyboardProxy* keyboard, MouseProxy* mouse);

    // Registers a region that can respond to hovers and clicks. The (x, y)
    // position of the region is with respect to the position of the active panel.
    // If this widget will consume keyboard input, it needs to be stated here. Widgets
    // only consume keyboard events while in focus.
    WidgetInfo Register(const std::string& name, const Maths::vec4& region);

    // Returns the region offsetted by the position of the current
    // panel if there is one, or region otherwise.
    Maths::vec4 ApplyOffset(const Maths::vec4& region);

    Texture* GetWhite() { return &d_white; }

    // Returns the current panels main draw command. Asserts if there is no current panel.
    DrawCommand& GetDrawCommand();

    // Submits an extra draw command for the current panel. Asserts if there is no current panel.
    void SubmitDrawCommand(const DrawCommand& cmd);

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    void StartPanel(const std::string& name,
                    Maths::vec4* region,
                    bool draggable,
                    bool clickable);
    void EndPanel();

    std::size_t GetClicked() const { return d_clicked; }
};

}