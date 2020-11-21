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

    KeyboardProxy d_keyboard;
    MouseProxy d_mouse;

    UIEngine d_engine;
    Font d_font;

public:
    SimpleUI(Window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

    Font* GetFont() { return &d_font; }
    
    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    void StartPanel(const std::string& name, glm::vec4* region, PanelType type);
    void EndPanel();

    void Quad(const glm::vec4& colour,
              const glm::vec4& region);

    // TODO: Simplify the text API, it's a bit messy currently
    void Text(const std::string& text,
              float size,
              const glm::vec4& region,
              const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    void Text(const std::string& text,
              float size,
              const glm::vec2& position,
              const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    void TextModifiable(const std::string& name,
                        const glm::vec4& region,
                        std::string* text,
                        const glm::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    bool Button(const std::string& name,
                const glm::vec4& region);

    bool Checkbox(const std::string& name,
                  const glm::vec4& region,
                  bool* value);

    void Slider(const std::string& name,
                const glm::vec4& region,
                float* value, float min, float max);

    void Dragger(const std::string& name,
                 const glm::vec4& region,
                 float* value, float speed);

    void Image(const std::string& name,
               std::shared_ptr<Texture> image,
               const glm::vec2& position);

};

}