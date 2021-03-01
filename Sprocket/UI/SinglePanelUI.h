#pragma once
#include "Window.h"
#include "Shader.h"
#include "Event.h"
#include "StreamBuffer.h"
#include "Font.h"
#include "UIEngine.h"

#include <vector>
#include <unordered_map>
#include <optional>
#include <chrono>
#include <deque>
#include <string_view>

namespace Sprocket {

struct SinglePanelUITheme
{
    glm::vec4 backgroundColour;
    glm::vec4 baseColour;
    glm::vec4 hoveredColour;
    glm::vec4 clickedColour;
};

class SinglePanelUI
{
    Window* d_window;

    SinglePanelUITheme d_theme;

    UIEngine d_engine;
    Font d_font;

public:
    SinglePanelUI(Window* window);

    const SinglePanelUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SinglePanelUITheme& theme) { d_theme = theme; }

    Font* GetFont() { return &d_font; }

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    bool Button(std::string_view name,
                const glm::vec4& region);

    void Slider(std::string_view name,
                const glm::vec4& region,
                float* value, float min, float max);

    void Box(const glm::vec4& quad, const glm::vec4& colour);
};

}