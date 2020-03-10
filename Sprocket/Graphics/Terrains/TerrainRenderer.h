#pragma once
#include "Core/Window.h"
#include "Graphics/Terrains/Terrain.h"
#include "Graphics/Light.h"
#include "Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderOptions.h"

namespace Sprocket {

class TerrainRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Shader used to draw entities.

public:
    TerrainRenderer(Window* window);

    void update(const Camera& camera,
                const Lights& lights,
                const RenderOptions& options = RenderOptions());

    void draw(const Terrain& terrain);
};

}