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

    // OBSERVER MODE
    Sprocket::FirstPersonCamera d_observerCamera;

    // EDITOR MODE
    Sprocket::ThirdPersonCamera d_editorCamera;
    
    // RENDERING
    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::TerrainRenderer d_terrainRenderer;

    Sprocket::PostProcessor   d_postProcessor;

    Sprocket::Camera* d_camera;
        // Pointer to the current camera based on the mode.

    Sprocket::PerspectiveLens d_lens;
        // Perspective Projection for all modes.

    // WORLD
    Sprocket::Terrain d_terrain;

    // Entity management and systems
    Sprocket::EntityManager d_entityManager;
    Sprocket::BasicSelector d_selector;

    // Additional world setup
    Sprocket::DirectionalLight d_sun;
    float                      d_sunAngle;
    
    // LAYER DATA
    bool d_paused = false;
    bool d_mouseRequired = false;

    friend class EscapeMenu;
    friend class EditorUI;

public:
    WorldLayer(const Sprocket::CoreSystems& core);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(float dt) override;
};