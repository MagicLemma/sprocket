#pragma once
#include <Sprocket/UI/Font/Font.h>
#include <Sprocket/UI/UIEngine.h>

#include <glm/glm.hpp>

#include <string_view>

namespace spkt {

class window;
class event;

struct SinglePanelUITheme
{
    glm::vec4 backgroundColour;
    glm::vec4 baseColour;
    glm::vec4 hoveredColour;
    glm::vec4 clickedColour;
};

class SinglePanelUI
{
    window* d_window;

    SinglePanelUITheme d_theme;

    spkt::UIEngine d_engine;
    spkt::Font d_font;

public:
    SinglePanelUI(window* window);

    const SinglePanelUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SinglePanelUITheme& theme) { d_theme = theme; }

    Font* GetFont() { return &d_font; }

    void on_event(event& event);
    void on_update(double dt);

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