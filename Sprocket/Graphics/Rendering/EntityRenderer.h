#pragma once
#include "Window.h"
#include "Entity.h"
#include "Light.h"
#include "Shader.h"
#include "Texture.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Components.h"

namespace Sprocket {

class EntityRenderer
{
    Window*         d_window;
    ModelManager*   d_modelManager;
    TextureManager* d_textureManager;

    Shader  d_shader;

    bool d_renderColliders;

    void DrawModel    (const Entity& entity);
    void DrawCollider (const Entity& entity);

    void DrawBox      (const Entity& entity);
    void DrawSphere   (const Entity& entity);
    void DrawCapsule  (const Entity& entity);

public:
    EntityRenderer(
        Window* window,
        ModelManager* modelManager,
        TextureManager* textureManager
    );

    // If true, wireframes of the physics colliders will be rendered.
    void RenderColliders(bool value);

    bool ShowColliders() const { return d_renderColliders; }

    void BeginScene(const Entity& camera, const Lights& lights, Scene& scene);
    void BeginScene(const Maths::mat4& proj, const Maths::mat4& view, const Lights& lights, Scene& scene);

    void EnableShadows(const Texture& shadowMap, const Maths::mat4& lightProjView);

    void Draw(const Entity& entity);

    Shader& GetShader() { return d_shader; }
};

}