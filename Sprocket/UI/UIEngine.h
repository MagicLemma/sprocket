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
    std::vector<BufferVertex> vertices;
    std::vector<u32> indices;
    const Texture* texture;
};

struct Panel
{
    std::string name;
    std::size_t hash;
    Maths::vec4 region;

    std::vector<QuadData> widgetRegions;

    // Render data
    std::vector<BufferVertex> quadVertices;
    std::vector<u32> quadIndices;

    std::vector<BufferVertex> textVertices;
    std::vector<u32> textIndices;

    std::vector<DrawCommand> extraCommands;
};

struct WidgetTimes
{
    double hoveredTime = 0.0;
    double unhoveredTime = 0.0;
    double clickedTime = 0.0;
    double unclickedTime = 0.0;
};

struct WidgetInfo
// When registering a new widget with the UIEngine, the callers gets
// this struct back. It contains, in seconds, the amount of time that
// the current widget has been (un)hovered and (un)clicked, among
// other pieces of info. The caller can then use this information to
// decide how the widget should be rendered.
{
    double mouseOver = 0.0;
    double sinceHovered = 0.0f;
    double sinceUnhovered = 0.0;

    double mouseDown = 0.0;
    double sinceClicked = 0.0;
    double sinceUnlicked = 0.0;
    
    bool onClick = false;
    bool onHover = false;

    bool focused = false;

    // The keys pressed since the last frame. This is guaranteed to be empty
    // if the current widget is not focused.
    std::vector<int> keyPresses;

    // The region of the widget converted to screen space coords.
    Maths::vec4 quad;
};

enum class Alignment
{
    LEFT,
    CENTRE,
    RIGHT
};

class UIEngine
{
    Window* d_window;

    KeyboardProxy* d_keyboard;
    MouseProxy* d_mouse;

    Font d_font;

    // Rendering code    
    Shader d_shader;
    StreamBuffer d_buffer;
    
    // Panel info 
    std::unordered_map<std::size_t, Panel> d_panels;
    Panel* d_currentPanel = nullptr;
    std::deque<std::size_t> d_panelOrder;

    std::size_t d_hovered = 0;
    std::size_t d_clicked = 0;
        // Hashes of the currently hovered/clicked widgets.

    double d_hoveredTime = 0.0;
    double d_clickedTime = 0.0;
        // Times (in seconds) that the current widgets have been
        // hovered/selected.

    std::unordered_map<std::size_t, WidgetTimes> d_widgetTimes;
        // Hash -> time map keeping track of the last time each
        // widget was unselected. Used to calculate the unhovered
        // and unclicked times.

    double d_time = 0.0;
        // A steadily increasing timer used to set the unselected
        // times in the maps above.

    std::size_t d_onClick = 0;
    std::size_t d_onHover = 0;
        // Stores which widget has been clicked/hovered so that it can
        // be acted on next frame. These are consumed when retrieved.

    std::size_t d_focused = 0; // TODO: Expand with timings and add to WidgetInfo
    std::vector<int> d_keyPresses;
        // A vector of key presses that happened since last frame. This will be
        // given to the currently focused widget.

public:
    UIEngine(Window* window, KeyboardProxy* keyboard, MouseProxy* mouse);

    // Registers a region that can respond to hovers and clicks. The (x, y)
    // position of the region is with respect to the position of the active panel.
    // If this widget will consume keyboard input, it needs to be stated here. Widgets
    // only consume keyboard events while in focus.
    WidgetInfo Register(const std::string& name, const Maths::vec4& region);

    Maths::vec4 ApplyOffset(const Maths::vec4& region);
        // Returns the region offsetted by the position of the current
        // panel if there is one, or region otherwise.

    Font& GetFont() { return d_font; }

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    bool StartPanel(const std::string& name,
                    Maths::vec4* region,
                    bool* active,
                    bool* draggable,
                    bool* clickable);
    void EndPanel();

    void DrawQuad(
        const Maths::vec4& colour,
        const Maths::vec4& quad
    );

    // Basic draw functions, does not take panelling into account.
    void DrawText(
        const std::string& text,
        float size,
        const Maths::vec4& quad,
        Alignment alignment = Alignment::CENTRE,
        const Maths::vec4& colour = {1.0, 1.0, 1.0, 1.0}
    );

    void SubmitDrawCommand(const DrawCommand& cmd);

    std::size_t GetClicked() const { return d_clicked; }
};

}