#pragma once
#include "Window.h"
#include "Model2D.h"
#include "Shader.h"
#include "RenderOptions.h"
#include "Quad.h"
#include "UiComponent.h"

#include <memory>

namespace Sprocket {

class DisplayRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Shader used to draw entities.

    Model2D d_quadModel;
        // A basic quad for rendering UI.

public:
    DisplayRenderer(Window* window);

    void draw(const Quad& quad) const;
    void draw(const UiComponent& button) const;
};

}