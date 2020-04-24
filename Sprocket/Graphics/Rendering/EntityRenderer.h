#pragma once
#include "Window.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include "Lens.h"
#include "Shader.h"
#include "RenderOptions.h"

#include "Texture.h"

namespace Sprocket {

class EntityRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Basic shader used to draw entities.

    Shader d_mirrorShader;
        // Shader used to mirror-like entities.

    void drawColliders(const Entity& entity);
    Model3D d_cube;
    Model3D d_sphere;
    Model3D d_hemisphere;
    Model3D d_cylinder;

public:
    EntityRenderer(Window* window);

    void update(const Camera& camera,
                const Lens& lens,
                const Lights& lights,
                const RenderOptions& options = RenderOptions());
        // To be called on every frame, all non-entity updates of the
        // shader should be carried out here.

    void draw(const Entity& entity,
              bool renderColliders = false);
};

}