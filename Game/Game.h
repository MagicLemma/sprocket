#pragma once
#include <Game/day_night_cycle.h>
#include <Game/game_grid.h>

#include <Game/ecs/scene.h>

#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/post_processor.h>
#include <Sprocket/Graphics/Rendering/pbr_renderer.h>
#include <Sprocket/Graphics/shadow_map.h>
#include <Sprocket/UI/DevUI.h>
#include <Sprocket/UI/SimpleUI.h>

#include <memory>
#include <random>
#include <string_view>

namespace spkt {
    class window;
    class event;
}

enum class mode { PLAYER, EDITOR };

class Game
{
    spkt::window* d_window;
    spkt::asset_manager d_assetManager;
    
    mode d_mode;

    game::scene d_scene;

    game::entity d_camera;
    game::entity d_worker;
    
    // RENDERING
    spkt::pbr_renderer d_entityRenderer;
    spkt::post_processor d_post_processor;

    // Additional world setup
    day_night_cycle d_cycle;

    spkt::shadow_map d_shadowMap;

    spkt::SimpleUI d_hoveredEntityUI;
    
    // LAYER DATA
    bool d_paused = false;
    std::string d_sceneFile = "";

    // Editor UI
    spkt::DevUI d_devUI;

    // Escape Menu
    spkt::SimpleUI d_escapeMenu;

public:
    Game(spkt::window* window);

    void on_event(spkt::event& event);
    void on_update(double dt);
    void on_render();

    void load_scene(std::string_view file);

    std::pair<glm::mat4, glm::mat4> get_proj_view_matrices() const;
};