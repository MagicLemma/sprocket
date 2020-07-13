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
#include <stack>
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

    std::vector<DrawCommand> d_commands;
        // One for the window and an extra one for each window.
    std::size_t d_commandIndex = 0;

    std::stack<Maths::vec2> d_offset;
        // The top of the stack represents the offset that the
        // regions should be drawn from. If the stack is empty, the
        // widgets are drawn with respect to the main window.

    Maths::vec4 ApplyOffset(const Maths::vec4& region);
    void DrawQuad(const Maths::vec4& colour, const Maths::vec4& quad);
    void DrawText(const std::string& text, const Maths::vec4& quad);

public:
    SimpleUI(Window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    void StartWindow(const std::string& name, Maths::vec4* region);
    void EndWindow();

    void Quad(const Maths::vec4& colour,
              const Maths::vec4& region);

    void Text(const std::string& text,
              const Maths::vec4& region);

    bool Button(const std::string& name,
                const Maths::vec4& region);

    void Slider(const std::string& name,
                const Maths::vec4& region,
                float* value, float min, float max);

    void Dragger(const std::string& name,
                 const Maths::vec4& region,
                 float* value, float speed);
};

}