#pragma once
#include <anvil/ecs/ecs.h>
#include <anvil/ecs/scene.h>

#include <Sprocket/Core/window.h>
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/cube_map.h>
#include <Sprocket/Graphics/Rendering/pbr_renderer.h>
#include <Sprocket/Graphics/Rendering/skybox_renderer.h>
#include <Sprocket/UI/console.h>

#include <memory>
#include <random>

namespace anvil {

class runtime
{
    spkt::window*       d_window;
    spkt::asset_manager d_asset_manager;

    // Rendering
    spkt::pbr_renderer    d_scene_renderer;
    spkt::skybox_renderer d_skybox_renderer;

    // Scene
    anvil::scene   d_scene;
    spkt::cube_map d_skybox;
    
    anvil::entity d_runtime_camera;

    // Console
    spkt::simple_ui d_ui;
    spkt::console  d_console;
    std::string    d_command_line;
    bool           d_console_active = false;

public:
    runtime(spkt::window* window);

    void on_event(spkt::event& event);
    void on_update(double dt);
    void on_render();
};

}