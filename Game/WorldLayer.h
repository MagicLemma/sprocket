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
    Sprocket::Window*      d_window;
    Sprocket::AssetManager d_assetManager;
    
    Mode d_mode;

    // Entity management and systems
    std::shared_ptr<Sprocket::CameraSystem>  d_cameraSystem;
    std::shared_ptr<Sprocket::BasicSelector> d_selector;
    std::shared_ptr<Sprocket::ScriptRunner>  d_scriptRunner;
    std::shared_ptr<Sprocket::PathFollower>  d_pathFollower;
    std::shared_ptr<Sprocket::GameGrid>      d_gameGrid;
    
    std::shared_ptr<Sprocket::Scene> d_scene;

    Sprocket::Entity d_camera;
    Sprocket::Entity d_worker;
    
    // RENDERING
    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::PostProcessor   d_postProcessor;

    // Additional world setup
    Sprocket::CircadianCycle d_cycle;

    Sprocket::ShadowMap d_shadowMap;

    Sprocket::SimpleUI d_hoveredEntityUI;
    
    // LAYER DATA
    bool d_paused = false;

    std::string d_sceneFile = "";

    friend class EscapeMenu;
    friend class EditorUI;

public:
    WorldLayer(Sprocket::Window* window);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
    void OnRender() override;

    void SaveScene();
    void LoadScene(const std::string& sceneFile);

    void AddTree(const glm::ivec2& pos);

    void AddRockBase(const glm::ivec2& pos,
                     const std::string& tex,
                     const std::string& name);
    
    void AddRock(const glm::ivec2& pos);
    void AddIron(const glm::ivec2& pos);
    void AddTin(const glm::ivec2& pos);
    void AddMithril(const glm::ivec2& pos);
};