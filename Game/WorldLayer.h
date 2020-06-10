#pragma once
#include "GameGrid.h"

#include <Sprocket.h>

#include <memory>
#include <random>

enum class Mode { PLAYER, EDITOR };

class WorldLayer : public Sprocket::Layer
{
    Mode d_mode;

    Sprocket::ThirdPersonCamera d_camera;
    
    // RENDERING
    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::TerrainRenderer d_terrainRenderer;

    Sprocket::PostProcessor   d_postProcessor;

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

    Sprocket::MouseProxy d_mouse;

    GameGrid d_gameGrid;
    
    // LAYER DATA
    bool d_paused = false;

    friend class EscapeMenu;
    friend class EditorUI;

public:
    WorldLayer(const Sprocket::CoreSystems& core);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(float dt) override;
};