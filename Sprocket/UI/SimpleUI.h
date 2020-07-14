#pragma once
#include "Window.h"
#include "Shader.h"
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "StreamBuffer.h"
#include "Font.h"
#include "UIEngine.h"

#include <vector>
#include <unordered_map>
#include <optional>
#include <chrono>

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

struct DrawCommand
{
    std::vector<BufferVertex> quadVertices;
    std::vector<unsigned int> quadIndices;

    std::vector<BufferVertex> textVertices;
    std::vector<unsigned int> textIndices;
};

struct PanelInfo
{
    std::size_t hash;
    std::string name;
    Maths::vec4 region;
};

class SimpleUI
{
    Window* d_window;

    SimpleUITheme d_theme;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

    UIEngine d_engine;

    Font d_font;

    // Rendering code    
    Shader d_shader;
    StreamBuffer d_buffer;

    void DrawQuad(const Maths::vec4& colour, const Maths::vec4& quad);
    void DrawText(const std::string& text, const Maths::vec4& quad);
        // Basic draw functions, does not take panelling into account.
    
    // Panel info 
    std::optional<PanelInfo> d_currentPanel = {};
    std::unordered_map<std::size_t, DrawCommand> d_commands;

    Maths::vec4 ApplyOffset(const Maths::vec4& region);
        // Returns the region offsetted by the position of the current
        // panel if there is one, or region otherwise.

    std::string MangleName(const std::string& name);
        // Returns <panel name>##<name> if there is a current panel,
        // or name otherwise.

public:
    SimpleUI(Window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

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
};

}