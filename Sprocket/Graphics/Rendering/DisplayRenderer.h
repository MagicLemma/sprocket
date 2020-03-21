#pragma once
#include "Window.h"
#include "Model2D.h"
#include "Shader.h"
#include "RenderOptions.h"
#include "Quad.h"
#include "UiComponent.h"
#include "Image.h"

// Remove after finding the bug with Image
#include "Model2D.h"
#include "Texture.h"

#include <memory>

namespace Sprocket {

class DisplayRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_quadShader;
        // Shader used to draw simple quads.

    Shader d_imageShader;
        // Shader used to draw images (textured quads).

    Model2D d_quad;

public:
    DisplayRenderer(Window* window);

    void draw(const Quad& quad) const;
    void draw(const UiComponent& button) const;
    void draw(const Image& image) const;
};

}