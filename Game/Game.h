#pragma once
#include "GameGrid.h"
#include "CircadianCycle.h"
#include "PathFollower.h"

#include <Sprocket.h>

#include <memory>
#include <random>
#include <string_view>

enum class Mode { PLAYER, EDITOR };

class WorldLayer
{
    Sprocket::Window*      d_window;
    Sprocket::AssetManager d_assetManager;
    
    Mode d_mode;

    Sprocket::Scene d_scene;

    Sprocket::ecs::Entity d_camera;
    Sprocket::ecs::Entity d_worker;
    
    // RENDERING
    Sprocket::Scene3DRenderer  d_entityRenderer;
    Sprocket::PostProcessor   d_postProcessor;

    // Additional world setup
    Sprocket::CircadianCycle d_cycle;

    Sprocket::ShadowMap d_shadowMap;

    Sprocket::SimpleUI d_hoveredEntityUI;
    
    // LAYER DATA
    bool d_paused = false;
    std::string d_sceneFile = "";

    // Editor UI
    Sprocket::DevUI d_devUI;

    // Escape Menu
    Sprocket::SimpleUI d_escapeMenu;

public:
    WorldLayer(Sprocket::Window* window);

    void OnEvent(Sprocket::Event& event);
    void OnUpdate(double dt);
    void OnRender();

    void LoadScene(std::string_view file);

    void AddTree(const glm::ivec2& pos);

    void AddRockBase(const glm::ivec2& pos,
                     std::string_view tex,
                     std::string_view name);
    
    void AddRock(const glm::ivec2& pos);
    void AddIron(const glm::ivec2& pos);
    void AddTin(const glm::ivec2& pos);
    void AddMithril(const glm::ivec2& pos);
};