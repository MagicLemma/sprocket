#pragma once
#include <Sprocket/UI/Font/font.h>
#include <Sprocket/UI/ui_engine.h>

#include <glm/glm.hpp>

#include <string>
#include <string_view>

namespace spkt {

class window;
class event;

struct SimpleUITheme
{
    glm::vec4 backgroundColour;
    glm::vec4 baseColour;
    glm::vec4 hoveredColour;
    glm::vec4 clickedColour;
};

class SimpleUI
{
    window* d_window;

    spkt::SimpleUITheme d_theme;

    spkt::ui_engine d_engine;
    spkt::font      d_font;

public:
    SimpleUI(window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

    spkt::font* GetFont() { return &d_font; }
    
    void on_event(event& event);
    void on_update(double dt);

    void StartFrame();
    void EndFrame();

    void StartPanel(std::string_view name, glm::vec4* region, panel_type type);
    void EndPanel();

    void Quad(const glm::vec4& colour,
              const glm::vec4& region);

    // TODO: Simplify the text API, it's a bit messy currently
    void Text(std::string_view text,
              float size,
              const glm::vec4& region,
              const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    void Text(std::string_view text,
              float size,
              const glm::vec2& position,
              const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    void TextModifiable(std::string_view name,
                        const glm::vec4& region,
                        std::string* text,
                        const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    bool Button(std::string_view name,
                const glm::vec4& region);

    bool Checkbox(std::string_view name,
                  const glm::vec4& region,
                  bool* value);

    void Slider(std::string_view name,
                const glm::vec4& region,
                float* value, float min, float max);

    void Dragger(std::string_view name,
                 const glm::vec4& region,
                 float* value, float speed);

    void Image(std::string_view name,
               const texture* image,
               const glm::vec2& position);

};

}