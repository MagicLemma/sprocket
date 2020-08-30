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

class EntityRenderer
{
    Window*         d_window;
    ModelManager*   d_modelManager;
    TextureManager* d_textureManager;

    Shader  d_shader;

    std::unique_ptr<VertexArray> d_vao;

    void DrawModel    (const Entity& entity);

public:
    EntityRenderer(
        Window* window,
        ModelManager* modelManager,
        TextureManager* textureManager
    );

    void Draw(const Entity& camera, const Lights& lights, Scene& scene);
    void Draw(const Maths::mat4& proj, const Maths::mat4& view, const Lights& lights, Scene& scene);

    void EnableShadows(const Texture& shadowMap, const Maths::mat4& lightProjView);

    Shader& GetShader() { return d_shader; }
};

}