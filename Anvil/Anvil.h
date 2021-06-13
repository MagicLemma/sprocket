#pragma once
#include <Sprocket.h>
#include "Camera.h"

#include "Inspector.h"

#include <memory>
#include <random>

namespace Sprocket {

class Anvil
{
    Window*      d_window;
    AssetManager d_asset_manager;
    
    Camera d_editor_camera;

    // Rendering
    Scene3DRenderer d_entity_renderer;
    SkyboxRenderer d_skybox_renderer;
    ColliderRenderer d_collider_renderer;

    ParticleManager d_particle_manager;

    FrameBuffer d_viewport;
    glm::ivec2 d_viewport_size;

    DevUI d_ui;
    bool d_is_viewport_hovered = false;
    bool d_is_viewport_focused = false;

    // Scene
    std::string d_sceneFile = "Resources/Anvil.yaml";
    std::shared_ptr<Scene> d_activeScene;
    std::shared_ptr<Scene> d_scene;
    spkt::entity d_runtimeCamera;

    // Additional world setup
    CubeMap d_skybox;
    float  d_sunAngle = 45.0f;
    
    // LAYER DATA
    bool d_paused = false;
    bool d_mouseRequired = false;

    bool d_playingGame = false;
    bool d_showColliders = false;

    void add_entity_to_list(const spkt::entity& entity);
    spkt::entity d_selected = spkt::null;

    // Panels
    Inspector      d_inspector;

    void material_ui(std::string& texture);

    glm::mat4 get_view_matrix() const;
    glm::mat4 get_proj_matrix() const;

public:
    Anvil(Window* window);

    void on_event(ev::Event& event);
    void on_update(double dt);
    void on_render();

    // Remove these
    void OnEvent(ev::Event& event) { on_event(event); }
    void OnUpdate(double dt) { on_update(dt); }
    void OnRender() { on_render(); }

    spkt::entity selected() { return d_selected; }
    void set_selected(spkt::entity e) { d_selected = e; }
    void clear_selected() { d_selected = spkt::null; }

    bool is_game_running() const { return d_playingGame; }

    Camera& editor_camera() { return d_editor_camera; }
    Window* window() { return d_window; }

    std::shared_ptr<Scene> active_scene() { return d_activeScene; }
};

}