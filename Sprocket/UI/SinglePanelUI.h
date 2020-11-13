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
#include <deque>

namespace Sprocket {

struct SinglePanelUITheme
{
    Maths::vec4 backgroundColour;
    Maths::vec4 baseColour;
    Maths::vec4 hoveredColour;
    Maths::vec4 clickedColour;
};

class SinglePanelUI
{
    Window* d_window;

    SinglePanelUITheme d_theme;

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

    UIEngine d_engine;

public:
    SinglePanelUI(Window* window);

    const SinglePanelUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SinglePanelUITheme& theme) { d_theme = theme; }

    const Font* GetFont() { return d_engine.GetFont(); }

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    bool Button(const std::string& name,
                const Maths::vec4& region);

    void Slider(const std::string& name,
                const Maths::vec4& region,
                float* value, float min, float max);

    void Box(const Maths::vec4& quad, const Maths::vec4& colour);
};

}