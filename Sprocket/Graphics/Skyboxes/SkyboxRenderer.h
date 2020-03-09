#pragma once
#include "Core/Window.h"
#include "Graphics/Skyboxes/Skybox.h"
#include "FirstPersonCamera.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderOptions.h"


namespace Sprocket {

class SkyboxRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Shader used to draw entities.

public:
    SkyboxRenderer(Window* window);

    void draw(const Skybox& entity,
              const FirstPersonCamera& camera,
              const RenderOptions& options = RenderOptions());
};

}