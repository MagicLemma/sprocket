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
#include "ShadowMapRenderer.h"
#include "PostProcessor.h"

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

    ShadowMapRenderer d_shadowMap;
    bool d_renderShadows = true;

    PostProcessor d_postProcessor;
    bool d_postProcessEffects = false;

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
    
    void OnUpdate(EntityManager& manager, double dt, bool active) override;
    void OnEvent(Event& event) override;

    void SetCamera(Entity& camera) { d_camera = camera; }

    void EnableShadows(bool val) { d_renderShadows = val; }
    void EnablePostProcessor(bool val) { d_postProcessEffects = val; }

    ShadowMapRenderer& GetShadowMap() { return d_shadowMap; }
    PostProcessor& GetPostProcessor() { return d_postProcessor; }
    Shader& GetShader() { return d_shader; }

    Lights& GetLights() { return d_lights; }

    template <typename T>
    void AddEffect() { d_postProcessor.AddEffect<T>(); }
};

}