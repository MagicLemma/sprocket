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

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

    UIEngine d_engine;

    Font d_font;

    // Rendering code    
    Shader d_shader;
    StreamBuffer d_buffer;

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