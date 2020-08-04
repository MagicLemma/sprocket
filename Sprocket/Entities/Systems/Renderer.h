#pragma once
#include "Window.h"
#include "Entity.h"
#include "Light.h"
#include "Shader.h"
#include "Texture.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Components.h"
#include "EntitySystem.h"

namespace Sprocket {

class EntityManager;

class Renderer : public EntitySystem
{
    Window*         d_window;
    ModelManager*   d_modelManager;
    TextureManager* d_textureManager;

    Shader  d_shader;

    Entity d_camera;
    Lights d_lights;

    void DrawModel    (const Entity& entity);
    void DrawCollider (const Entity& entity);

    void DrawBox      (const Entity& entity);
    void DrawSphere   (const Entity& entity);
    void DrawCapsule  (const Entity& entity);

public:
    Renderer(
        Window* window,
        ModelManager* modelManager,
        TextureManager* textureManager
    );
    
    virtual void OnUpdate(EntityManager& manager, double dt) override;

    void SetCamera(Entity& camera) { d_camera = camera; }
    void SetLights(const Lights& lights) { d_lights = lights; }
};

}