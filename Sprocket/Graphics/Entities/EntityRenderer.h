#pragma once
#include "Core/Window.h"
#include "Graphics/Entities/Entity.h"
#include "Graphics/Light.h"
#include "Graphics/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderOptions.h"

namespace Sprocket {

class EntityRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Shader used to draw entities.

public:
    EntityRenderer(Window* window);

    void draw(const Entity& entity,
              const Camera& camera,
              const Lights& lights,
              const RenderOptions& options = RenderOptions());
};

}