#pragma once
#include <glm/glm.hpp>

#include <Sprocket/UI/Font/Font.h>
#include <Sprocket/UI/UIEngine.h>

#include <string>
#include <string_view>

namespace spkt {

class Window;
namespace ev { class Event; }

struct SimpleUITheme
{
    glm::vec4 backgroundColour;
    glm::vec4 baseColour;
    glm::vec4 hoveredColour;
    glm::vec4 clickedColour;
};

class SimpleUI
{
    Window* d_window;

    SimpleUITheme d_theme;

    UIEngine d_engine;
    Font d_font;

public:
    SimpleUI(Window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

    Font* GetFont() { return &d_font; }
    
    void on_event(ev::Event& event);
    void on_update(double dt);

    void StartFrame();
    void EndFrame();

    void StartPanel(std::string_view name, glm::vec4* region, PanelType type);
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
               const Texture* image,
               const glm::vec2& position);

};

}