#pragma once
#include "Window.h"
#include "Model2D.h"
#include "Shader.h"
#include "RenderOptions.h"
#include "Quad.h"
#include "Widget.h"

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

    Shader d_textureShader;
        // Shader used to draw a textured Quad.

    Model2D d_quad;

public:
    DisplayRenderer(Window* window);

    void update() const;

    void draw(const Widget& widget) const;
    void draw(const Widget& widget, const VisualQuad& quad) const;

    void draw(const VisualQuad& quad) const;
};

}