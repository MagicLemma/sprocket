#pragma once
#include "Window.h"
#include "Shader.h"
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "StreamBuffer.h"
#include "BufferLayout.h"
#include "Font.h"

#include <vector>
#include <unordered_map>
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

class SimpleUI
{
    Window* d_window;

    SimpleUITheme d_theme;

    Shader d_shader;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

    StreamBuffer d_buffer;
    BufferLayout d_bufferLayout;

    Font d_font;

    int d_clicked; // ID of the clicked widget.

    // Used to construct the VBO each frame.
    std::vector<BufferVertex> d_quadVertices;
    std::vector<unsigned int> d_quadIndices;

    std::vector<BufferVertex> d_textVertices;
    std::vector<unsigned int> d_textIndices;

public:
    SimpleUI(Window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    void Quad(const Maths::vec4& colour,
              const Maths::vec4& region);

    bool Button(int id, const std::string& name,
                const Maths::vec4& region);

    void Slider(int id, const std::string& name,
                const Maths::vec4& region,
                float* value, float min, float max);

    void Text(const std::string& text,
              const Maths::vec4& region);
};

}