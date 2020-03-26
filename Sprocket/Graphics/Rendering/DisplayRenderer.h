#pragma once
#include "Window.h"
#include "Model2D.h"
#include "Shader.h"
#include "RenderOptions.h"
#include "Quad.h"
#include "Font.h"

// Remove after finding the bug with Image
#include "Model2D.h"
#include "Texture.h"

#include <memory>

namespace Sprocket {

class DisplayRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_colourShader;
        // Shader used to draw a coloured Quad.

    Shader d_characterShader;
        // Shader used to draw characters.

    Model2D d_quad;

public:
    DisplayRenderer(Window* window);

    void update() const;

    void draw(const Quad& quad, const QuadVisuals& visuals) const;

    void draw(int character,
              const Font& font,
              const Maths::vec2& position,
              float size,
              const Maths::vec3& colour) const;

    void draw(const std::string& sentence,
              const Font& font,
              const Maths::vec2& position,
              float size,
              const Maths::vec3& colour) const;
};


}