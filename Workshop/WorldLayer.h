#pragma once
#include <Sprocket.h>

#include <memory>
#include <random>

enum class Mode
{
    PLAYER,
    OBSERVER,
    EDITOR
};

class WorldLayer : public Sprocket::Layer
{
    Mode d_mode;

    // PLAYER MODE
    Sprocket::Entity* d_playerCamera;

    // OBSERVER MODE
    Sprocket::Entity* d_observerCamera;

    // EDITOR MODE
    Sprocket::ThirdPersonCamera d_editorCamera;
    
    // RENDERING
    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::SkyboxRenderer  d_skyboxRenderer;

    Sprocket::PostProcessor   d_postProcessor;

    Sprocket::Camera* d_camera;
        // Pointer to the current camera based on the mode.

    Sprocket::PerspectiveLens d_lens;
        // Perspective Projection for all modes.

    // WORLD
    // Entity management and systems
    Sprocket::EntityManager  d_entityManager;
    Sprocket::PlayerMovement d_playerMovement;
    Sprocket::PhysicsEngine  d_physicsEngine;
    Sprocket::Selector       d_selector;
    Sprocket::ScriptRunner   d_scriptRunner;

    // Additional world setup
    Sprocket::Skybox d_skybox;
    Sprocket::Lights d_lights;
    float            d_sunAngle = 45.0f;
    
    // LAYER DATA
    bool d_paused = false;
    bool d_mouseRequired = false;

    friend class EscapeMenu;
    friend class EditorUI;

public:
    WorldLayer(const Sprocket::CoreSystems& core);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
};