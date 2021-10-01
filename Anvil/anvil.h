#pragma once
#include <Anvil/Camera.h>
#include <Anvil/Inspector.h>

#include <Sprocket/Core/events.h>
#include <Sprocket/Core/Window.h>
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/CubeMap.h>
#include <Sprocket/Graphics/frame_buffer.h>
#include <Sprocket/Graphics/Rendering/geometry_renderer.h>
#include <Sprocket/Graphics/Rendering/Scene3DRenderer.h>
#include <Sprocket/Graphics/Rendering/SkyboxRenderer.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/UI/DevUI.h>

#include <memory>
#include <random>
#include <memory>

class Anvil
{
    spkt::window*      d_window;
    spkt::asset_manager d_asset_manager;
    
    Camera d_editor_camera;

    // Rendering
    spkt::Scene3DRenderer d_entity_renderer;
    spkt::SkyboxRenderer d_skybox_renderer;
    spkt::geometry_renderer d_geometry_renderer;

    spkt::frame_buffer d_viewport;
    glm::ivec2 d_viewport_size;

    spkt::DevUI d_ui;
    bool d_is_viewport_hovered = false;
    bool d_is_viewport_focused = false;

    // Scene
    std::string d_sceneFile = "Resources/Anvil.yaml";
    std::shared_ptr<spkt::scene> d_activeScene;
    std::shared_ptr<spkt::scene> d_scene;
    spkt::entity d_runtimeCamera;

    // Additional world setup
    spkt::CubeMap d_skybox;
    float  d_sunAngle = 45.0f;
    
    // LAYER DATA
    bool d_paused = false;
    bool d_mouseRequired = false;

    bool d_playingGame = false;
    bool d_showColliders = false;

    spkt::entity d_selected = spkt::null;

    // Panels
    Inspector d_inspector;

    void material_ui(std::string& texture);

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_proj_matrix() const;

public:
    Anvil(spkt::window* window);

    void on_event(spkt::event& event);
    void on_update(double dt);
    void on_render();

    spkt::entity selected() { return d_selected; }
    void set_selected(spkt::entity e) { d_selected = e; }
    void clear_selected() { d_selected = spkt::null; }

    bool is_game_running() const { return d_playingGame; }

    Camera& editor_camera() { return d_editor_camera; }
    spkt::window* window() { return d_window; }

    std::shared_ptr<spkt::scene> active_scene() { return d_activeScene; }
};