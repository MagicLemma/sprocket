#pragma once
#include "Window.h"
#include "Skybox.h"
#include "Entity.h"
#include "Shader.h"


namespace Sprocket {

class SkyboxRenderer
{
    Window* d_window;
        // Non-owning pointer to the window to draw in.

    Shader d_shader;
        // Shader used to draw entities.

public:
    SkyboxRenderer(Window* window);

    void Draw(const Skybox& skybox, const Entity& camera);
    void Draw(const Skybox& skybox, const Maths::mat4& proj, const Maths::mat4& view);
};

}