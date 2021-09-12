#include "Game.h"

#include <Game/grid_helpers.h>
#include <Game/Palette.h>
#include <Game/PathCalculator.h>

#include <Sprocket/Audio/Listener.h>
#include <Sprocket/Core/events.h>
#include <Sprocket/Core/Window.h>
#include <Sprocket/Scene/camera.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/loader.h>
#include <Sprocket/Scene/Systems/basic_systems.h>
#include <Sprocket/Scene/Systems/game_grid.h>
#include <Sprocket/UI/ImGuiXtra.h>
#include <Sprocket/Core/input_codes.h>
#include <Sprocket/Core/log.h>
#include <Sprocket/Core/input_codes.h>

#include <cmath>

using namespace spkt;

namespace {

void SunInfoPanel(DevUI& ui, CircadianCycle& cycle)
{
    ImGui::Begin("Sun");

    if (ImGui::Button("Start")) { cycle.Start(); }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) { cycle.Stop(); }
    ImGui::Separator();

    if (cycle.IsRunning()) { // If running, be able to change the speed.
        float speed = cycle.GetSpeed();
        ImGui::SliderFloat("Speed", &speed, 1.0f, 1000.0f, "%.3f");
        cycle.SetSpeed(speed);
    }
    else { // If not running, be able to set manually
        float angle = cycle.GetAngle();
        ImGui::DragFloat("Angle", &angle, 0.01f);
        cycle.SetAngle(angle);
    }
    
    ImGui::Separator();
    ImGuiXtra::Text(cycle.ToString12Hour());
    ImGui::End();
}

void ShaderInfoPanel(DevUI& ui, shader& shader)
{
    static std::string compileStatus;

    ImGui::Begin("Shader");
    if(ImGui::Button("Recompile")) {
        bool result = shader.reload();
        compileStatus ="Shader compile:";
        compileStatus += (result? " SUCCESS": " FAILURE");
    }

    ImGui::Text(compileStatus.c_str());
    
    bool closed = true;
    if (ImGui::CollapsingHeader("Vertex")) {
        closed = false;
        ImGuiXtra::MultilineTextModifiable("", &shader.vertex_source());
    }
    if (ImGui::CollapsingHeader("Frag")) {
        closed = false;
        ImGuiXtra::MultilineTextModifiable("", &shader.fragment_source());
    }
    
    if(closed) {
        compileStatus.clear();
    }

    ImGui::End();
}

}

Game::Game(spkt::window* window) 
    : d_window(window)
    , d_scene()
    , d_assetManager()
    , d_mode(Mode::PLAYER)
    , d_entityRenderer(&d_assetManager)
    , d_post_processor(d_window->Width(), d_window->Height())
    , d_shadowMap(&d_assetManager)
    , d_hoveredEntityUI(d_window)
    , d_devUI(window)
    , d_escapeMenu(window)
{
    using namespace spkt;

    load_scene("Resources/Scene.yaml");

    SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_hoveredEntityUI.SetTheme(theme);
    d_escapeMenu.SetTheme(theme);    

    d_cycle.SetAngle(3.14195f);

    auto& registry = d_scene.registry;
    auto sun_entity = registry.find<SunComponent>();
    auto& sun = registry.get<SunComponent>(sun_entity);
    sun.direction = d_cycle.GetSunDir();

    d_post_processor.add_effect(
        "Resources/Shaders/GaussH.vert", "Resources/Shaders/Gauss.frag"
    );
    d_post_processor.add_effect(
        "Resources/Shaders/GaussV.vert", "Resources/Shaders/Gauss.frag"
    );
    d_post_processor.add_effect(
        "Resources/Shaders/Negative.vert", "Resources/Shaders/Negative.frag"
    );
}

void Game::load_scene(std::string_view file)
{
    auto& registry = d_scene.registry;
    
    spkt::add_singleton(registry);
    spkt::game_grid_system_init(registry);
    spkt::load_registry_from_file(std::string(file), registry);
    
    d_scene.systems = {
        spkt::game_grid_system,
        spkt::script_system,
        spkt::camera_system,
        spkt::path_follower_system,
        spkt::clear_events_system
    };

    d_paused = false;
    d_sceneFile = file;

    d_worker = registry.find<spkt::NameComponent>([&](spkt::entity entity) {
        return registry.get<spkt::NameComponent>(entity).name == "Worker";
    });

    d_camera = registry.find<spkt::NameComponent>([&](spkt::entity entity) {
        return registry.get<spkt::NameComponent>(entity).name == "Camera";
    });

    auto& cam = get_singleton<spkt::CameraSingleton>(registry);
    cam.camera_entity = d_camera;

    assert(registry.valid(d_worker));
    assert(registry.valid(d_camera));
}

void Game::on_event(spkt::event& event)
{
    using namespace spkt;

    // Escape Menu event handling
    if (auto data = event.get_if<keyboard_pressed_event>()) {
        if (!event.is_consumed() && data->key == Keyboard::ESC) {
            d_paused = !d_paused;
            event.consume();
        }
    }

    auto& registry = d_scene.registry;
    auto tile_entity = registry.find<TileMapSingleton>();
    const auto& tiles = registry.get<TileMapSingleton>(tile_entity).tiles;

    if (d_paused) {
        d_escapeMenu.on_event(event);
        event.consume();
    }

    // Editor UI event handling
    if (d_mode == Mode::EDITOR) {
        d_devUI.on_event(event);
    }

    // Game World event handling
    d_hoveredEntityUI.on_event(event);

    if (auto data = event.get_if<window_resized_event>()) {
        d_post_processor.set_screen_size(data->width, data->height);
    }

    if (auto data = event.get_if<mouse_pressed_event>()) {
        auto& tr = registry.get<Transform3DComponent>(d_camera);
        if (data->mods & KeyModifier::CTRL) {
            glm::vec3 cameraPos = tr.position;
            glm::vec3 direction = Maths::GetMouseRay(
                d_window->GetMousePos(),
                (float)d_window->Width(),
                (float)d_window->Height(),
                spkt::make_view(registry, d_camera),
                spkt::make_proj(registry, d_camera)
            );

            float lambda = -cameraPos.y / direction.y;
            glm::vec3 mousePos = cameraPos + lambda * direction;
            mousePos.y = 0.5f;
            
            auto& path = registry.get<PathComponent>(d_worker);

            if (data->button == Mouse::LEFT) {
                path.markers.clear();
                auto pos = registry.get<Transform3DComponent>(d_worker).position;
                if (glm::distance(pos, mousePos) > 1.0f) {
                    const auto& grid = get_singleton<GameGridSingleton>(registry);
                    path.markers = make_astar_path(
                        pos,
                        mousePos,
                        [&](const glm::ivec2& pos) {
                            auto it = tiles.find(pos);
                            spkt::entity entity = it != tiles.end() ? it->second : spkt::null;
                            return registry.valid(entity);
                        }
                    );
                } else {
                    path.markers.push_back(mousePos);
                }
                event.consume();
            }
            else if (data->button == Mouse::RIGHT) {
                path.markers.clear();
                event.consume();
            }
        }
    }

    if (!event.is_consumed()) {
        d_scene.on_event(event);
    }
}

void Game::on_update(double dt)
{
    using namespace spkt;
    auto& registry = d_scene.registry;

    spkt::set_listener(registry, d_camera);

    d_hoveredEntityUI.on_update(dt);
    if (!d_paused) {
        d_cycle.on_update(dt);
    }
    
    auto sun_entity = registry.find<SunComponent>();
    auto& sun = registry.get<SunComponent>(sun_entity);
    float factor = (-d_cycle.GetSunDir().y + 1.0f) / 2.0f;
    float facSq = factor * factor;
    auto skyColour = (1.0f - facSq) * NAVY_NIGHT + facSq * LIGHT_BLUE;
    d_window->SetClearColour(skyColour);
    if (d_cycle.IsDay()) {
        sun.direction = d_cycle.GetSunDir();
        sun.colour = {1.0, 0.945, 0.789};
    }
    else {
        sun.direction = -d_cycle.GetSunDir();
        sun.colour = {0.5, 0.57, 0.98};
    }
    sun.brightness = 2.0f * std::abs(glm::cos(d_cycle.GetAngle()));
    sun.direction = glm::normalize(sun.direction);

    if (!d_paused) {
        d_scene.on_update(dt);
    }

    d_devUI.on_update(dt);
    d_escapeMenu.on_update(dt);
}

void Game::on_render()
{
    using namespace spkt;
    
    const auto& game_grid = get_singleton<GameGridSingleton>(d_scene.registry);
    auto& registry = d_scene.registry;

    // Create the Shadow Map
    float lambda = 5.0f; // TODO: Calculate the floor intersection point
    auto& tc = registry.get<Transform3DComponent>(d_camera);
    glm::vec3 target = tc.position + lambda * Maths::Forwards(tc.orientation);
    auto sun = registry.find<SunComponent>();
    d_shadowMap.draw(
        registry,
        registry.get<SunComponent>(sun).direction,
        target
    );

    if (d_paused) {
        d_post_processor.start_frame();
    }

    d_entityRenderer.EnableShadows(d_shadowMap);
    d_entityRenderer.Draw(registry, d_camera);

    if (d_paused) {
        d_post_processor.end_frame();
    }

    if (!d_paused) {
        auto tile_entity = registry.find<TileMapSingleton>();
        const auto& tiles = registry.get<TileMapSingleton>(tile_entity).tiles;

        d_hoveredEntityUI.StartFrame();

        auto mouse = d_window->GetMousePos();
        float w = (float)d_window->Width();
        float h = (float)d_window->Height();

        if (game_grid.clicked_square.has_value()) {
            auto it = tiles.find(game_grid.clicked_square.value());
            spkt::entity e = (it != tiles.end()) ? it->second : spkt::null;

            float width = 0.15f * w;
            float height = 0.6f * h;
            float x = w - width;
            float y = ((1.0f - 0.6f) / 2) * h;

            glm::vec4 region{x, y, width, height};
            d_hoveredEntityUI.StartPanel("Selected", &region, PanelType::UNCLICKABLE);
                
            auto pos = game_grid.clicked_square.value();
            if (d_hoveredEntityUI.Button("+Tree", {0, 0, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_tree(registry, pos);
            }

            if (d_hoveredEntityUI.Button("+Rock", {0, 60, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_rock(registry, pos);
            }

            if (d_hoveredEntityUI.Button("+Iron", {0, 120, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_iron(registry, pos);
            }

            if (d_hoveredEntityUI.Button("+Tin", {0, 180, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_tin(registry, pos);
            }

            if (d_hoveredEntityUI.Button("+Mithril", {0, 240, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_mithril(registry, pos);
            }

            if (d_hoveredEntityUI.Button("Clear", {0, 300, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
            }

            d_hoveredEntityUI.EndPanel();
        }

        auto it = tiles.find(game_grid.hovered_square);
        spkt::entity e = (it != tiles.end()) ? it->second : spkt::null;
        if (registry.valid(e)) {
            float width = 200;
            float height = 50;
            float x = std::min(mouse.x - 5, w - width - 10);
            float y = std::min(mouse.y - 5, h - height - 10);

            glm::vec4 region{x, y, width, height};
            d_hoveredEntityUI.StartPanel("Hovered", &region, PanelType::UNCLICKABLE);
            std::string name = "Unnamed";
            if (registry.has<NameComponent>(e)) {
                name = registry.get<NameComponent>(e).name;
            }
            d_hoveredEntityUI.Text(name, 36.0f, {0, 0, width, height});
            d_hoveredEntityUI.EndPanel();
        }

        d_hoveredEntityUI.EndFrame();
    }

    if (d_mode == Mode::EDITOR) {
        d_devUI.StartFrame();

        glm::mat4 view = spkt::make_view(registry, d_camera);
        glm::mat4 proj = spkt::make_proj(registry, d_camera);

        SunInfoPanel(d_devUI, d_cycle);
        ShaderInfoPanel(d_devUI, d_entityRenderer.GetShader());

        ImGui::Begin("Shadow Map");
        ImGuiXtra::Image(d_shadowMap.get_texture(), 500.0f);
        ImGui::End();

        ImGui::ShowDemoWindow();

        d_devUI.EndFrame();
    }

    // Rest of the rendering is the escape menu, which should only get rendered when paused.
    if (!d_paused) {
        return;
    }

    float w = (float)d_window->Width();
    float h = (float)d_window->Height();

    d_escapeMenu.StartFrame();
    static bool showVolume = false;

    glm::vec4 mainRegion{0.0f, 0.0f, w * 0.3f, h};
    d_escapeMenu.StartPanel("Main", &mainRegion, PanelType::CLICKABLE);

    d_escapeMenu.Text("Menu", 48.0f, {0.0f, 0.0f, w * 0.3f, 100});

    glm::vec4 buttonRegion = {w * 0.025f, 100, w * 0.25f, 50};

    if (d_escapeMenu.Button("Toggle Dev UI", buttonRegion)) {
        switch (d_mode) {
            case Mode::PLAYER: {
                d_mode = Mode::EDITOR;
            } break;
            case Mode::EDITOR: {
                d_mode = Mode::PLAYER;
            } break;
        }
    }

    buttonRegion.y += 2 * 60;
    static float value1 = 27.0f;
    d_escapeMenu.Slider("Slider", buttonRegion, &value1, 0, 100);

    buttonRegion.y += 60;
    float angle = d_cycle.GetAngle();
    d_escapeMenu.Dragger("Time of Day", buttonRegion, &angle, 0.001f);
    d_cycle.SetAngle(angle);

    buttonRegion.y += 60;
    d_escapeMenu.Checkbox("Volume Panel", buttonRegion, &showVolume);

    buttonRegion.y += 60;
    if (d_escapeMenu.Button("Reload", buttonRegion)) {
        load_scene(d_sceneFile);
    }

    buttonRegion.y += 60;
    if (d_escapeMenu.Button("Save", buttonRegion)) {
        spkt::log::info("Saving to {}", d_sceneFile);
        spkt::save_registry_to_file(d_sceneFile, registry);
        spkt::log::info("Done!");
    }
    
    d_escapeMenu.EndPanel();
    
    static glm::vec4 shape{w/2 - 200, 100, 400, 500};
    if (showVolume) {
        d_escapeMenu.StartPanel("VolumePanel", &shape, PanelType::DRAGGABLE);
        d_escapeMenu.Text("Volume", 48.0f, {0, 0, 400, 100});

        float volume = spkt::get_master_volume();
        d_escapeMenu.Slider("Master Volume", {10, 100, 400 - 20, 50}, &volume, 0.0, 100.0);
        spkt::set_master_volume(volume);
        
        d_escapeMenu.EndPanel();
    }

    static glm::vec4 shape2{w/2 + 300, 100, 400, 500};
    d_escapeMenu.StartPanel("Button Panel", &shape2, PanelType::DRAGGABLE);
    d_escapeMenu.Text("Buttons", 36.0f, {0, 0, 400, 100});
    glm::vec4 buttonQuad{10, 100, 400 - 20, 50};
    if (d_escapeMenu.Button("Button 1", buttonQuad)) {
        spkt::log::warn("Warn");
        spkt::log::info("Info");
        spkt::log::error("Error");
        spkt::log::fatal("Fatal");
    }
    buttonQuad.y += 60;
    d_escapeMenu.Button("Button 2", buttonQuad);
    buttonQuad.y += 60;
    d_escapeMenu.Button("Button 3", buttonQuad);
    buttonQuad.y += 60;
    d_escapeMenu.Button("Button 4", buttonQuad);
    buttonQuad.y += 60;
    d_escapeMenu.Button("Button 5", buttonQuad);
    buttonQuad.y += 60;
    buttonQuad.z = buttonQuad.w;

    static bool valA = false;
    d_escapeMenu.Checkbox("A", buttonQuad, &valA);
    buttonQuad.x += buttonQuad.z + 10.0f;

    static bool valB = true;
    d_escapeMenu.Checkbox("B", buttonQuad, &valB);
    buttonQuad.x += buttonQuad.z + 10.0f;

    d_escapeMenu.EndPanel();
    d_escapeMenu.EndFrame();
}