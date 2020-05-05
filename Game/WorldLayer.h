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
    Sprocket::PlayerBasedCamera d_playerCamera;

    // OBSERVER MODE
    Sprocket::FirstPersonCamera d_observerCamera;

    // EDITOR MODE
    Sprocket::ThirdPersonCamera d_editorCamera;
    
    // RENDERING
    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::TerrainRenderer d_terrainRenderer;
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

    // Additional world setup
    Sprocket::Skybox             d_skybox;
    std::vector<Sprocket::Light> d_lights;
    
    // LAYER DATA
    bool d_paused = false;
    bool d_mouseRequired = false;

    friend class EscapeMenu;
    friend class EditorUI;

public:
    WorldLayer(Sprocket::Accessor& accessor);

    bool handleEventImpl(const Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;
};