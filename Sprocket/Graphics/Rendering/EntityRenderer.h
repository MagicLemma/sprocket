#pragma once
#include "Window.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include "Lens.h"
#include "Shader.h"
#include "Texture.h"

namespace Sprocket {

class EntityRenderer
{
    Window* d_window;
    Shader  d_shader;

    bool d_renderColliders;

    void DrawModel    (const Entity& entity);
    void DrawCollider (const Entity& entity);

    void DrawBox      (const Entity& entity, const BoxCollider* collider);
    void DrawSphere   (const Entity& entity, const SphereCollider* collider);
    void DrawCapsule  (const Entity& entity, const CapsuleCollider* collider);

public:
    EntityRenderer(Window* window);

    void RenderColliders(bool value);

    bool ShowColliders() const { return d_renderColliders; }

    void BeginScene(const Maths::mat4& view, const Maths::mat4& proj, const Lights& lights);

    void BeginScene(const Camera& camera, const Lens& lens, const Lights& lights);
        // To be called on every frame, all non-entity updates of the
        // shader should be carried out here.

    void BeginScene(const Entity& camera, const Lights& light);

    void EnableShadows(const Texture& shadowMap, const Maths::mat4& lightProjView);


    void Draw(const Entity& entity);
};

}