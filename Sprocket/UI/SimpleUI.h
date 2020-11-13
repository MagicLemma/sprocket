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

public:
    SimpleUI(Window* window);

    const SimpleUITheme& GetTheme() const { return d_theme; }
    void SetTheme(const SimpleUITheme& theme) { d_theme = theme; }

    const Font* GetFont() { return d_engine.GetFont(); }
    
    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void StartFrame();
    void EndFrame();

    bool StartPanel(const std::string& name,
                    Maths::vec4* region,
                    bool active,
                    bool draggable,
                    bool clickable);
    void EndPanel();

    void Quad(const Maths::vec4& colour,
              const Maths::vec4& region);

    // TODO: Simplify the text API, it's a bit messy currently
    void Text(const std::string& text,
              float size,
              const Maths::vec4& region,
              const Maths::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    void Text(const std::string& text,
              float size,
              const Maths::vec2& position,
              const Maths::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    void TextModifiable(const std::string& name,
                        const Maths::vec4& region,
                        std::string* text,
                        const Maths::vec4& colour = {1.0, 1.0, 1.0, 1.0});

    bool Button(const std::string& name,
                const Maths::vec4& region);

    bool Checkbox(const std::string& name,
                  const Maths::vec4& region,
                  bool* value);

    void Slider(const std::string& name,
                const Maths::vec4& region,
                float* value, float min, float max);

    void Dragger(const std::string& name,
                 const Maths::vec4& region,
                 float* value, float speed);

    void Image(const std::string& name,
               const Texture& image,
               const Maths::vec2& position);

    // TEMP: Hashes are implementation details, just is just for debugging
    std::size_t GetClicked() const { return d_engine.GetClicked(); }
};

}