#pragma once
#include <game/day_night_cycle.h>
#include <game/game_grid.h>

#include <game/ecs/scene.h>

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

class app
{
    spkt::window* d_window;
    spkt::asset_manager d_asset_manager;
    
    mode d_mode;

    game::scene d_scene;

    game::entity d_camera;
    game::entity d_worker;
    
    // RENDERING
    spkt::pbr_renderer d_scene_renderer;
    spkt::post_processor d_post_processor;

    // Additional world setup
    day_night_cycle d_cycle;

    spkt::shadow_map d_shadow_map;

    spkt::SimpleUI d_hovered_entity_ui;
    
    // LAYER DATA
    bool d_paused = false;
    std::string d_scene_file = "";

    // Editor UI
    spkt::DevUI d_dev_ui;

    // Escape Menu
    spkt::SimpleUI d_escape_menu;

public:
    app(spkt::window* window);

    void on_event(spkt::event& event);
    void on_update(double dt);
    void on_render();

    void load_scene(std::string_view file);

    std::pair<glm::mat4, glm::mat4> get_proj_view_matrices() const;
};