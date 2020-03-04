#pragma once
#include "Core/Window.h"
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderOptions.h"

namespace Sprocket {

class DisplayRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Shader used to draw entities.

public:
    DisplayRenderer(Window* window);

    void draw(ModelPtr model,
              const RenderOptions& options = RenderOptions());
};

}