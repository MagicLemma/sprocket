#pragma once
#include "Window.h"
#include "Shader.h"
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "StreamBuffer.h"
#include "Font.h"

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

struct SimpleUITheme
{
    Maths::vec4 backgroundColour;
    Maths::vec4 baseColour;
    Maths::vec4 hoveredColour;
    Maths::vec4 clickedColour;
};

struct QuadData
{
    std::size_t hash;
    Maths::vec4 region;
};

struct DrawCommand
{
    std::vector<BufferVertex> vertices;
    std::vector<unsigned int> indices;
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
    std::vector<unsigned int> quadIndices;

    std::vector<BufferVertex> textVertices;
    std::vector<unsigned int> textIndices;

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
};

class SimpleUI
{
    Window* d_window;

    SimpleUITheme d_theme;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

    Font d_font;

    // Rendering code    
    Shader d_shader;
    StreamBuffer d_buffer;

    void DrawQuad(const Maths::vec4& colour, const Maths::vec4& quad);
    void DrawText(const std::string& text, float size, const Maths::vec4& quad);
        // Basic draw functions, does not take panelling into account.
    
    // Panel info 
    std::unordered_map<std::size_t, Panel> d_panels;
    Panel* d_currentPanel = nullptr;
    std::deque<std::size_t> d_panelOrder;

    Maths::vec4 ApplyOffset(const Maths::vec4& region);
        // Returns the region offsetted by the position of the current
        // panel if there is one, or region otherwise.

    std::string MangleName(const std::string& name);
        // Returns <panel name>##<name> if there is a current panel,
        // or name otherwise.

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

public:
    SimpleUI(Window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

    WidgetInfo RegisterWidget(const std::string& name,
                              const Maths::vec4& region);

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    bool StartPanel(const std::string& name,
                    Maths::vec4* region,
                    bool* active,
                    bool* draggable);
    void EndPanel();

    void Quad(const Maths::vec4& colour,
              const Maths::vec4& region);

    void Text(const std::string& text,
              float size,
              const Maths::vec4& region);

    bool Button(const std::string& name,
                const Maths::vec4& region);

    bool Checkbox(const std::string& name,
                  const Maths::vec4& region,
                  bool* value);

    void Slider(const std::string& name,
                const Maths::vec4& region,
                float* value, float min, float max);

    void Dragger(const std::string& name,
                 const Maths::vec4& region,
                 float* value, float speed);

    void Image(const std::string& name,
               const Texture& image,
               const Maths::vec2& position);
};

}