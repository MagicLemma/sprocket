#pragma once
#include "GameGrid.h"
#include "CircadianCycle.h"

#include <Sprocket.h>

#include <memory>
#include <random>

enum class Mode { PLAYER, EDITOR };

class WorldLayer : public Sprocket::Layer
{
    Mode d_mode;

    Sprocket::Entity* d_camera;
    
    // RENDERING
    Sprocket::EntityRenderer  d_entityRenderer;

    Sprocket::PostProcessor   d_postProcessor;

    // MODELLING
    Sprocket::ModelManager d_modelManager;

    // Entity management and systems
    Sprocket::EntityManager d_entityManager;
    Sprocket::BasicSelector d_selector;
    Sprocket::ScriptRunner  d_scriptRunner;

    // Additional world setup
    Sprocket::Lights         d_lights;
    Sprocket::CircadianCycle d_cycle;

    Sprocket::ShadowMapRenderer d_shadowMapRenderer;

    Sprocket::MouseProxy d_mouse;

    GameGrid d_gameGrid;
    
    // LAYER DATA
    bool d_paused = false;

    friend class EscapeMenu;
    friend class EditorUI;

public:
    WorldLayer(const Sprocket::CoreSystems& core);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
};