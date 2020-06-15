#pragma once
#include "Shader.h"
#include "Window.h"
#include "Light.h"
#include "StaticCamera.h"
#include "OrthographicLens.h"
#include "Entity.h"
#include "ShadowBox.h"

#include <memory>

namespace Sprocket {

class ShadowMapRenderer
{
    Window* d_window;
    Shader  d_shader;

public:
    ShadowMapRenderer(Window* window);

    void OnUpdate(const DirectionalLight& light);
        // Updates the camera and lens.

    void Draw(const Entity& entity);
        // Draw the entity.
};

}