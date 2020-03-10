#pragma once
#include "Core/Window.h"
#include "Graphics/Entities/Entity.h"
#include "Graphics/Light.h"
#include "Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Skyboxes/Skybox.h"
#include "Graphics/RenderOptions.h"

namespace Sprocket {

class EntityRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Basic shader used to draw entities.

    Shader d_mirrorShader;
        // Shader used to mirror-like entities.

public:
    EntityRenderer(Window* window);

    void update(const Camera& camera,
                const Lights& lights,
                const RenderOptions& options = RenderOptions());
        // To be called on every frame, all non-entity updates of the
        // shader should be carried out here.

    void draw(const Entity& entity);
};

}