#pragma once
#include "Core/Window.h"
#include "Graphics/Modelling/Model2D.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderOptions.h"

#include <memory>

namespace Sprocket {

class DisplayRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Shader used to draw entities.

public:
    DisplayRenderer(Window* window);

    void draw(std::shared_ptr<Model2D> model,
              const RenderOptions& options = RenderOptions());
};

}