#pragma once
#include "Window.h"
#include "Entity.h"
#include "Light.h"
#include "Shader.h"
#include "Texture.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Components.h"
#include "VertexArray.h"

namespace Sprocket {

class ColliderRenderer
{
    Window* d_window;

    Shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;

public:
    ColliderRenderer(Window* window);

    void Draw(const Entity& camera, const Lights& lights, Scene& scene);
    void Draw(const Maths::mat4& proj, const Maths::mat4& view, const Lights& lights, Scene& scene);

    Shader& GetShader() { return d_shader; }
};

}