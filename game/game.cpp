#include "game.h"

#include <game/game_grid.h>
#include <game/grid_helpers.h>
#include <game/palette.h>
#include <game/script_system.h>
#include <game/path_calculator.h>
#include <game/ecs/ecs.h>
#include <game/ecs/loader.h>

#include <sprocket/Core/sound.h>
#include <sprocket/Core/events.h>
#include <sprocket/Core/input_codes.h>
#include <sprocket/Core/log.h>
#include <sprocket/Core/Window.h>
#include <sprocket/graphics/camera.h>
#include <sprocket/graphics/render_context.h>
#include <sprocket/graphics/Rendering/pbr_renderer.h>
#include <sprocket/ui/ImGuiXtra.h>
#include <sprocket/Utility/views.h>

#include <cmath>

using namespace spkt;

namespace {

template <typename T>
T& get_singleton(game::registry& reg)
{
    return reg.get<T>(reg.find<T>());
}

void clear_events_system(game::registry& registry, double dt)
{
    registry.destroy_if<game::Event>([](game::entity) { return true; });
}

void path_follower_system(game::registry& registry, double dt)
{
    for (auto [path, transform] : registry.view_get<game::PathComponent, game::Transform3DComponent>()) {
        if (path.markers.empty()) { continue; }
        
        glm::vec3 to_dest = path.markers.front() - transform.position;
        glm::vec3 direction = glm::normalize(to_dest);
        glm::vec3 advance = path.speed * (float)dt * direction;

        if (glm::length2(advance) < glm::length2(to_dest)) {
            transform.position += advance;
        } else {
            transform.position = path.markers.front();
            path.markers.pop_front();
        }
    }
}

void input_system_init(game::registry& registry, spkt::window* window)
{
    assert(window);
    auto singleton = registry.create();
    registry.emplace<game::Runtime>(singleton);
    auto& is = registry.emplace<game::InputSingleton>(singleton);
    is.input_store = std::make_shared<spkt::input_store>(window);
}

void input_system_on_event(game::registry& registry, spkt::event& event)
{
    auto singleton = registry.find<game::InputSingleton>();
    registry.get<game::InputSingleton>(singleton).input_store->on_event(event);
}

void input_system_end(game::registry& registry, double dt)
{
    auto singleton = registry.find<game::InputSingleton>();
    registry.get<game::InputSingleton>(singleton).input_store->end_frame();
}

void SunInfoPanel(imgui_ui& ui, day_night_cycle& cycle)
{
    ImGui::Begin("Sun");

    if (ImGui::Button("Start")) { cycle.start(); }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) { cycle.stop(); }
    ImGui::Separator();

    if (cycle.is_running()) { // If running, be able to change the speed.
        float speed = cycle.get_speed();
        ImGui::SliderFloat("Speed", &speed, 1.0f, 1000.0f, "%.3f");
        cycle.set_speed(speed);
    }
    else { // If not running, be able to set manually
        float angle = cycle.get_angle();
        ImGui::DragFloat("Angle", &angle, 0.01f);
        cycle.set_angle(angle);
    }
    
    ImGui::Separator();
    ImGuiXtra::Text(cycle.to_string());
    ImGui::End();
}

void ShaderInfoPanel(imgui_ui& ui, shader& shader)
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

void draw_scene(
    spkt::pbr_renderer& renderer,
    const game::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view)
{
    spkt::render_context rc;
    rc.face_culling(true);
    rc.depth_testing(true);
    renderer.begin_frame(proj, view);

    if (auto a = registry.find<game::AmbienceComponent>(); registry.valid(a)) {
        const auto& ambience = registry.get<game::AmbienceComponent>(a);
        renderer.set_ambience(ambience.colour, ambience.brightness);
    }

    if (auto s = registry.find<game::SunComponent>(); registry.valid(s)) {
        const auto& sun = registry.get<game::SunComponent>(s);
        renderer.set_sunlight(sun.colour, sun.direction, sun.brightness);
    }

    for (auto [lc, tc] : registry.view_get<game::LightComponent, game::Transform3DComponent>()
                         | std::views::take(spkt::MAX_NUM_LIGHTS))
    {
        renderer.add_light(tc.position, lc.colour, lc.brightness);
    }

    for (auto [mc, tc] : registry.view_get<game::StaticModelComponent, game::Transform3DComponent>()) {
        renderer.draw_static_mesh(
            tc.position, tc.orientation, tc.scale,
            mc.mesh, mc.material
        );
    }

    for (auto [mc, tc] : registry.view_get<game::AnimatedModelComponent, game::Transform3DComponent>()) {
        renderer.draw_animated_mesh(
            tc.position, tc.orientation, tc.scale,
            mc.mesh, mc.material,
            mc.animation_name, mc.animation_time
        );
    }

    renderer.end_frame();
}

}

app::app(spkt::window* window) 
    : d_window(window)
    , d_scene()
    , d_asset_manager()
    , d_mode(mode::PLAYER)
    , d_scene_renderer(&d_asset_manager)
    , d_post_processor(d_window->width(), d_window->height())
    , d_shadow_map(&d_asset_manager)
    , d_hovered_entity_ui(d_window)
    , d_dev_ui(window)
    , d_escape_menu(window)
{
    using namespace spkt;

    load_scene("Resources/Scene.yaml");

    simple_ui_theme theme;
    theme.background_colour = SPACE_DARK;
    theme.base_colour = CLEAR_BLUE;
    theme.hovered_colour = LIGHT_BLUE;
    theme.clicked_colour = GARDEN;
    d_hovered_entity_ui.set_theme(theme);
    d_escape_menu.set_theme(theme);    

    d_cycle.set_angle(3.14195f);

    auto& registry = d_scene.registry;
    auto sun_entity = registry.find<game::SunComponent>();
    auto& sun = registry.get<game::SunComponent>(sun_entity);
    sun.direction = d_cycle.sun_direction();

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

void app::load_scene(std::string_view file)
{
    auto& registry = d_scene.registry;
    
    input_system_init(registry, d_window);
    game_grid_system_init(registry);
    game::load_registry_from_file(std::string(file), registry);
    
    d_scene.systems = {
        game_grid_system,
        script_system,
        path_follower_system,
        clear_events_system,
        input_system_end
    };

    d_scene.event_handlers = {
        input_system_on_event
    };

    d_paused = false;
    d_scene_file = file;

    d_worker = registry.find<game::NameComponent>([&](game::entity entity) {
        return registry.get<game::NameComponent>(entity).name == "Worker";
    });

    d_camera = registry.find<game::NameComponent>([&](game::entity entity) {
        return registry.get<game::NameComponent>(entity).name == "Camera";
    });

    auto& cam = get_singleton<game::CameraSingleton>(registry);
    cam.camera_entity = d_camera;

    assert(registry.valid(d_worker));
    assert(registry.valid(d_camera));
}

void app::on_event(spkt::event& event)
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
    auto tile_entity = registry.find<game::TileMapSingleton>();
    const auto& tiles = registry.get<game::TileMapSingleton>(tile_entity).tiles;

    if (d_paused) {
        d_escape_menu.on_event(event);
        event.consume();
    }

    // Editor UI event handling
    if (d_mode == mode::EDITOR) {
        d_dev_ui.on_event(event);
    }

    // Game World event handling
    d_hovered_entity_ui.on_event(event);

    if (auto data = event.get_if<window_resized_event>()) {
        d_post_processor.set_screen_size(data->width, data->height);
    }

    if (auto data = event.get_if<mouse_pressed_event>()) {
        if (data->mods & KeyModifier::CTRL) {
            const auto& reg = registry;
            auto [tc, cc] = reg.get_all<game::Transform3DComponent, game::Camera3DComponent>(d_camera);

            glm::vec3 camera_position = tc.position;
            glm::vec3 direction = spkt::get_mouse_ray(
                d_window->mouse_position(),
                (float)d_window->width(),
                (float)d_window->height(),
                spkt::make_view(tc.position, tc.orientation, cc.pitch),
                spkt::make_proj(cc.fov)
            );

            float lambda = -camera_position.y / direction.y;
            glm::vec3 mouse_position = camera_position + lambda * direction;
            mouse_position.y = 0.5f;
            
            auto& path = registry.get<game::PathComponent>(d_worker);

            if (data->button == Mouse::LEFT) {
                path.markers.clear();
                auto pos = registry.get<game::Transform3DComponent>(d_worker).position;
                if (glm::distance(pos, mouse_position) > 1.0f) {
                    const auto& grid = get_singleton<game::GameGridSingleton>(registry);
                    path.markers = make_astar_path(
                        pos,
                        mouse_position,
                        [&](const glm::ivec2& pos) {
                            auto it = tiles.find(pos);
                            game::entity entity = it != tiles.end() ? it->second : game::null;
                            return registry.valid(entity);
                        }
                    );
                } else {
                    path.markers.push_back(mouse_position);
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

void app::on_update(double dt)
{
    using namespace spkt;
    auto& registry = d_scene.registry;

    const auto& transform = registry.get<game::Transform3DComponent>(d_camera);
    spkt::set_listener(transform.position, spkt::forwards(transform.orientation));

    d_hovered_entity_ui.on_update(dt);
    if (!d_paused) {
        d_cycle.on_update(dt);
    }
    
    auto sun_entity = registry.find<game::SunComponent>();
    auto& sun = registry.get<game::SunComponent>(sun_entity);
    float factor = (-d_cycle.sun_direction().y + 1.0f) / 2.0f;
    float factor2 = factor * factor;
    auto skyColour = (1.0f - factor2) * NAVY_NIGHT + factor2 * LIGHT_BLUE;
    d_window->set_clear_colour(skyColour);
    if (d_cycle.is_day()) {
        sun.direction = d_cycle.sun_direction();
        sun.colour = {1.0, 0.945, 0.789};
    }
    else {
        sun.direction = -d_cycle.sun_direction();
        sun.colour = {0.5, 0.57, 0.98};
    }
    sun.brightness = 2.0f * std::abs(glm::cos(d_cycle.get_angle()));
    sun.direction = glm::normalize(sun.direction);

    if (!d_paused) {
        d_scene.on_update(dt);
    }

    d_dev_ui.on_update(dt);
    d_escape_menu.on_update(dt);
}

void app::on_render()
{
    using namespace spkt;
    
    const auto& game_grid = get_singleton<game::GameGridSingleton>(d_scene.registry);
    auto& registry = d_scene.registry;

    // Create the Shadow Map
    float lambda = 5.0f; // TODO: Calculate the floor intersection point
    auto& tc = registry.get<game::Transform3DComponent>(d_camera);
    glm::vec3 target = tc.position + lambda * spkt::forwards(tc.orientation);
    auto sun = registry.find<game::SunComponent>();

    d_shadow_map.begin_frame(target, registry.get<game::SunComponent>(sun).direction);
    for (auto [mc, tc] : registry.view_get<game::StaticModelComponent, game::Transform3DComponent>()) {
        d_shadow_map.add_mesh(mc.mesh, tc.position, tc.orientation, tc.scale);
    }
    d_shadow_map.end_frame();

    if (d_paused) {
        d_post_processor.start_frame();
    }

    auto [proj, view] = get_proj_view_matrices();

    d_scene_renderer.enable_shadows(d_shadow_map);
    draw_scene(d_scene_renderer, registry, proj, view);

    if (d_paused) {
        d_post_processor.end_frame();
    }

    if (!d_paused) {
        auto tile_entity = registry.find<game::TileMapSingleton>();
        const auto& tiles = registry.get<game::TileMapSingleton>(tile_entity).tiles;

        d_hovered_entity_ui.start_frame();

        auto mouse = d_window->mouse_position();
        float w = (float)d_window->width();
        float h = (float)d_window->height();

        if (game_grid.clicked_square.has_value()) {
            auto it = tiles.find(game_grid.clicked_square.value());
            game::entity e = (it != tiles.end()) ? it->second : game::null;

            float width = 0.15f * w;
            float height = 0.6f * h;
            float x = w - width;
            float y = ((1.0f - 0.6f) / 2) * h;

            glm::vec4 region{x, y, width, height};
            d_hovered_entity_ui.start_panel("Selected", &region, panel_type::UNCLICKABLE);
                
            auto pos = game_grid.clicked_square.value();
            if (d_hovered_entity_ui.button("+Tree", {0, 0, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_tree(registry, pos);
            }

            if (d_hovered_entity_ui.button("+Rock", {0, 60, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_rock(registry, pos);
            }

            if (d_hovered_entity_ui.button("+Iron", {0, 120, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_iron(registry, pos);
            }

            if (d_hovered_entity_ui.button("+Tin", {0, 180, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_tin(registry, pos);
            }

            if (d_hovered_entity_ui.button("+Mithril", {0, 240, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
                add_mithril(registry, pos);
            }

            if (d_hovered_entity_ui.button("Clear", {0, 300, width, 50})) {
                if (registry.valid(e)) { registry.destroy(e); }
            }

            d_hovered_entity_ui.end_panel();
        }

        auto it = tiles.find(game_grid.hovered_square);
        game::entity e = (it != tiles.end()) ? it->second : game::null;
        if (registry.valid(e)) {
            float width = 200;
            float height = 50;
            float x = std::min(mouse.x - 5, w - width - 10);
            float y = std::min(mouse.y - 5, h - height - 10);

            glm::vec4 region{x, y, width, height};
            d_hovered_entity_ui.start_panel("Hovered", &region, panel_type::UNCLICKABLE);
            std::string name = "Unnamed";
            if (registry.has<game::NameComponent>(e)) {
                name = registry.get<game::NameComponent>(e).name;
            }
            d_hovered_entity_ui.text(name, 36.0f, {0, 0, width, height});
            d_hovered_entity_ui.end_panel();
        }

        d_hovered_entity_ui.end_frame();
    }

    if (d_mode == mode::EDITOR) {
        d_dev_ui.start_frame();

        const auto& reg = registry;
        auto [tc, cc] = reg.get_all<game::Transform3DComponent, game::Camera3DComponent>(d_camera);
        glm::mat4 view = spkt::make_view(tc.position, tc.orientation, cc.pitch);
        glm::mat4 proj = spkt::make_proj(cc.fov);

        SunInfoPanel(d_dev_ui, d_cycle);
        ShaderInfoPanel(d_dev_ui, d_scene_renderer.static_shader());

        ImGui::Begin("Shadow Map");
        ImGuiXtra::Image(d_shadow_map.get_texture(), 500.0f);
        ImGui::End();

        ImGui::ShowDemoWindow();

        d_dev_ui.end_frame();
    }

    // Rest of the rendering is the escape menu, which should only get rendered when paused.
    if (!d_paused) {
        return;
    }

    float w = (float)d_window->width();
    float h = (float)d_window->height();

    d_escape_menu.start_frame();
    static bool showVolume = false;

    glm::vec4 mainRegion{0.0f, 0.0f, w * 0.3f, h};
    d_escape_menu.start_panel("Main", &mainRegion, panel_type::CLICKABLE);

    d_escape_menu.text("Menu", 48.0f, {0.0f, 0.0f, w * 0.3f, 100});

    glm::vec4 button_region = {w * 0.025f, 100, w * 0.25f, 50};

    if (d_escape_menu.button("Toggle Dev UI", button_region)) {
        switch (d_mode) {
            case mode::PLAYER: {
                d_mode = mode::EDITOR;
            } break;
            case mode::EDITOR: {
                d_mode = mode::PLAYER;
            } break;
        }
    }

    button_region.y += 2 * 60;
    static float value1 = 27.0f;
    d_escape_menu.slider("Slider", button_region, &value1, 0, 100);

    button_region.y += 60;
    float angle = d_cycle.get_angle();
    d_escape_menu.dragger("Time of Day", button_region, &angle, 0.001f);
    d_cycle.set_angle(angle);

    button_region.y += 60;
    d_escape_menu.checkbox("Volume Panel", button_region, &showVolume);

    button_region.y += 60;
    if (d_escape_menu.button("Reload", button_region)) {
        load_scene(d_scene_file);
    }

    button_region.y += 60;
    if (d_escape_menu.button("Save", button_region)) {
        const auto entity_filter = [](const game::registry& reg, game::entity entity) {
            return !reg.has<game::Runtime>(entity);
        };

        spkt::log::info("Saving to {}", d_scene_file);
        game::save_registry_to_file(d_scene_file, registry, entity_filter);
        spkt::log::info("Done!");
    }
    
    d_escape_menu.end_panel();
    
    static glm::vec4 shape{w/2 - 200, 100, 400, 500};
    if (showVolume) {
        d_escape_menu.start_panel("VolumePanel", &shape, panel_type::DRAGGABLE);
        d_escape_menu.text("Volume", 48.0f, {0, 0, 400, 100});

        float volume = spkt::get_master_volume();
        d_escape_menu.slider("Master Volume", {10, 100, 400 - 20, 50}, &volume, 0.0, 100.0);
        spkt::set_master_volume(volume);
        
        d_escape_menu.end_panel();
    }

    static glm::vec4 shape2{w/2 + 300, 100, 400, 500};
    d_escape_menu.start_panel("Button Panel", &shape2, panel_type::DRAGGABLE);
    d_escape_menu.text("Buttons", 36.0f, {0, 0, 400, 100});
    glm::vec4 buttonQuad{10, 100, 400 - 20, 50};
    if (d_escape_menu.button("Button 1", buttonQuad)) {
        spkt::log::warn("Warn");
        spkt::log::info("Info");
        spkt::log::error("Error");
        spkt::log::fatal("Fatal");
    }
    buttonQuad.y += 60;
    d_escape_menu.button("Button 2", buttonQuad);
    buttonQuad.y += 60;
    d_escape_menu.button("Button 3", buttonQuad);
    buttonQuad.y += 60;
    d_escape_menu.button("Button 4", buttonQuad);
    buttonQuad.y += 60;
    d_escape_menu.button("Button 5", buttonQuad);
    buttonQuad.y += 60;
    buttonQuad.z = buttonQuad.w;

    static bool valA = false;
    d_escape_menu.checkbox("A", buttonQuad, &valA);
    buttonQuad.x += buttonQuad.z + 10.0f;

    static bool valB = true;
    d_escape_menu.checkbox("B", buttonQuad, &valB);
    buttonQuad.x += buttonQuad.z + 10.0f;

    d_escape_menu.end_panel();
    d_escape_menu.end_frame();
}

std::pair<glm::mat4, glm::mat4> app::get_proj_view_matrices() const
{
    const auto& reg = d_scene.registry;
    auto [tc, cc] = reg.get_all<game::Transform3DComponent, game::Camera3DComponent>(d_camera);
    return { 
        spkt::make_proj(cc.fov),
        spkt::make_view(tc.position, tc.orientation, cc.pitch)
    };
}