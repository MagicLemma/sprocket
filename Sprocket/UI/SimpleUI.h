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

struct WidgetInfo
{
    double hovered = 0.0;
    double unhovered = 0.0;

    double clicked = 0.0;
    double unclicked = 0.0;
    
    float onClick = false;
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

    std::size_t d_hovered = 0;
    double d_hoveredTime = 0.0;
    bool d_hoveredFlag = false;
    std::unordered_map<std::size_t, double> d_unhoveredTimes;
    
    std::size_t d_clicked = 0; // Hash of the clicked widget.
    double d_clickedTime = 0.0;
    bool d_clickedFlag = false;
    std::unordered_map<std::size_t, double> d_unclickedTimes;

    WidgetInfo GetWidgetInfo(const std::string& name,
                             const Maths::vec4& region);

    WidgetInfo buttonInfo; // TODO REMOVE

    double d_dt = 0.0;
    double d_time = 0.0;

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

    bool Button(const std::string& name,
                const Maths::vec4& region);

    void Slider(const std::string& name,
                const Maths::vec4& region,
                float* value, float min, float max);

    void Text(const std::string& text,
              const Maths::vec4& region);
};

}