#pragma once
#include "Window.h"
#include "Shader.h"
#include "Event.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "StreamBuffer.h"
#include "BufferLayout.h"

#include <vector>
#include <unordered_map>
#include <chrono>

namespace Sprocket {

struct QuadBufferVertex
{
    Maths::vec2 position;
    Maths::vec4 colour;
};

class SimpleUI
{
    Window* d_window;

    Shader d_shader;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

    StreamBuffer d_quadBuffer;
    BufferLayout d_quadBufferLayout;

    int d_clicked; // ID of the clicked widget.

    // Used to construct the VBO each frame.
    std::vector<QuadBufferVertex> d_quadBufferVertices;
    std::vector<unsigned int>     d_quadBufferIndices;

    void AddQuad(const Maths::vec2& pos,
                 float width, float height,
                 const Maths::vec4& colour);

public:
    SimpleUI(Window* window);

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    bool Button(int id, const std::string& name,
                float x, float y,
                float width, float height);
};

}