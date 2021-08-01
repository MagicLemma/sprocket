#pragma once
#include <Game/CircadianCycle.h>

#include <Sprocket/Graphics/AssetManager.h>
#include <Sprocket/Graphics/post_processor.h>
#include <Sprocket/Graphics/Rendering/Scene3DRenderer.h>
#include <Sprocket/Graphics/ShadowMap.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/Scene/Systems/GameGrid.h>
#include <Sprocket/UI/DevUI.h>
#include <Sprocket/UI/SimpleUI.h>

#include <memory>
#include <random>
#include <string_view>

namespace spkt {
    class Window;
    class event;
}

enum class Mode { PLAYER, EDITOR };

class Game
{
    spkt::Window* d_window;
    spkt::AssetManager d_assetManager;
    
    Mode d_mode;

    spkt::scene d_scene;

    spkt::entity d_camera;
    spkt::entity d_worker;
    
    // RENDERING
    spkt::Scene3DRenderer d_entityRenderer;
    spkt::post_processor d_post_processor;

    // Additional world setup
    spkt::CircadianCycle d_cycle;

    spkt::ShadowMap d_shadowMap;

    spkt::SimpleUI d_hoveredEntityUI;
    
    // LAYER DATA
    bool d_paused = false;
    std::string d_sceneFile = "";

    // Editor UI
    spkt::DevUI d_devUI;

    // Escape Menu
    spkt::SimpleUI d_escapeMenu;

public:
    Game(spkt::Window* window);

    void on_event(spkt::event& event);
    void on_update(double dt);
    void on_render();

    void load_scene(std::string_view file);
};