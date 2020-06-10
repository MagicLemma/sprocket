#pragma once
#include "Window.h"
#include "Terrain.h"
#include "Light.h"
#include "Camera.h"
#include "Lens.h"
#include "Shader.h"

namespace Sprocket {

class TerrainRenderer
{
    const unsigned int MAX_NUM_LIGHTS = 5;
    
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Shader used to draw entities.

public:
    TerrainRenderer(Window* window);

    void OnUpdate(const Camera& camera,
                const Lens& lens,
                const DirectionalLight& sun,
                const PointLights& lights = {});

    void Draw(const Terrain& terrain);
};

}