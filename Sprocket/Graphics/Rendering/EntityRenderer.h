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
    Shader  d_shader;

    bool d_wireFrame;
    bool d_depthTest;
    bool d_renderColliders;

    void drawColliders(const Entity& entity);

public:
    EntityRenderer(Window* window);

    void wireFrame(bool value);
    void depthTest(bool value);
    void renderColliders(bool value);

    void update(const Camera& camera, const Lens& lens, const Lights& lights);
        // To be called on every frame, all non-entity updates of the
        // shader should be carried out here.

    void draw(const Entity& entity);
};

}