#pragma once
#include "Window.h"
#include "Model2D.h"
#include "Shader.h"
#include "Quad.h"

#include <memory>
#include <unordered_map>

namespace Sprocket {

class DisplayRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_colourShader;
        // Shader used to draw a coloured Quad.

    Model2D d_quad;
        // Model that is bound to render quads.

public:
    DisplayRenderer(Window* window);

    void OnUpdate() const;

    void Draw(const Quad& quad) const;
    void Draw(const Quad& quad, const Model2D& model) const;
};


}