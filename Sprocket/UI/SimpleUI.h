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

#include <texture-atlas.h>
#include <texture-font.h>

namespace Sprocket {

struct BufferVertex
{
    Maths::vec2 position;
    Maths::vec4 colour;
    Maths::vec2 textureCoords = {0.0, 0.0};
};

struct SimpleUITheme
{
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

    Font d_texFont;

    int d_clicked; // ID of the clicked widget.

    // Used to construct the VBO each frame.
    std::vector<BufferVertex> d_quadVertices;
    std::vector<unsigned int> d_quadIndices;

    std::vector<BufferVertex> d_textVertices;
    std::vector<unsigned int> d_textIndices;

    void AddText(float x, float y, const std::string& text,
                 float size, float width, float height);

public:
    SimpleUI(Window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    void Quad(float x, float y,
              float width, float height,
              const Maths::vec4& colour);

    bool Button(int id, const std::string& name,
                float x, float y, float width, float height);

    void Slider(int id, const std::string& name,
                float x, float y, float width, float height,
                float* value, float min, float max);
};

}