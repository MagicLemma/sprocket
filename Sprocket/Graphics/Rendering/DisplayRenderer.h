#pragma once
#include "Window.h"
#include "Model2D.h"
#include "Shader.h"
#include "RenderOptions.h"
#include "Quad.h"
#include "Text.h"

#include <memory>
#include <unordered_map>

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
        // Model that is bound to render quads.

    std::unordered_map<Font, std::pair<std::string, std::string>> d_availableFonts;
        // A map of all available fonts. If a requested font is not in d_fonts,
        // the renderer will check here.

    std::unordered_map<Font, FontPackage> d_fonts;
        // Map of all loaded fonts to be accessible via the Font enum.

    FontPackage getFont(Font font);

public:
    DisplayRenderer(Window* window);

    void update() const;

    void draw(const Quad& quad) const;
    void draw(const Quad& quad, const Model2D& model) const;

    void draw(const Text& text);
};


}