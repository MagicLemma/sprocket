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

    void DrawBox      (const Entity& entity);
    void DrawSphere   (const Entity& entity);
    void DrawCapsule  (const Entity& entity);

public:
    ColliderRenderer(Window* window);

    void Draw(const Entity& camera, const Lights& lights, Scene& scene);
    void Draw(const Maths::mat4& proj, const Maths::mat4& view, const Lights& lights, Scene& scene);

    void Draw(const Entity& entity);

    Shader& GetShader() { return d_shader; }
};

}