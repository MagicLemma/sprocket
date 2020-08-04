#pragma once
#include "GameGrid.h"
#include "CircadianCycle.h"
#include "PathFollower.h"

#include <Sprocket.h>

#include <memory>
#include <random>

enum class Mode { PLAYER, EDITOR };

class WorldLayer : public Sprocket::Layer
{
    Mode d_mode;

    // Entity management and systems
    Sprocket::BasicSelector d_selector;
    Sprocket::ScriptRunner  d_scriptRunner;
    PathFollower            d_pathFollower;
    Sprocket::Renderer      d_renderer;
    Sprocket::EntityManager d_entityManager;

    Sprocket::Entity d_camera;
    Sprocket::Entity d_worker;
    
    // MODELLING
    Sprocket::ModelManager d_modelManager;

    // Additional world setup
    Sprocket::Lights         d_lights;
    Sprocket::CircadianCycle d_cycle;

    Sprocket::MouseProxy d_mouse;

    GameGrid d_gameGrid;
    Sprocket::SimpleUI d_hoveredEntityUI;
    
    // LAYER DATA
    bool d_paused = false;

    friend class EscapeMenu;
    friend class EditorUI;

public:
    WorldLayer(const Sprocket::CoreSystems& core);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;

    void AddTree(const Sprocket::Maths::ivec2& pos);

    void AddRockBase(const Sprocket::Maths::ivec2& pos,
                     const std::string& tex,
                     const std::string& name);
    
    void AddRock(const Sprocket::Maths::ivec2& pos);
    void AddIron(const Sprocket::Maths::ivec2& pos);
    void AddTin(const Sprocket::Maths::ivec2& pos);
    void AddMithril(const Sprocket::Maths::ivec2& pos);
};