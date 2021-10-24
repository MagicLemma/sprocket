#include "Anvil.h"

#include <Anvil/Inspector.h>
#include <Anvil/particle_system.h>
#include <Anvil/physics_system.h>
#include <Anvil/rendering.h>
#include <Anvil/systems.h>
#include <Anvil/scene_utils.h>
#include <Anvil/ecs/loader.h>
#include <Anvil/ecs/meta.h>

#include <Sprocket/Core/input_codes.h>
#include <Sprocket/Core/log.h>
#include <Sprocket/Graphics/camera.h>
#include <Sprocket/Graphics/material.h>
#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/UI/ImGuiXtra.h>
#include <Sprocket/Utility/file_browser.h>
#include <Sprocket/Utility/maths.h>
#include <Sprocket/Vendor/imgui/imgui.h>

#include <glm/glm.hpp>

#include <string_view>
#include <ranges>

namespace {

template <typename T>
T& get_singleton(anvil::registry& reg)
{
    return reg.get<T>(reg.find<T>());
}

std::string entiy_name(anvil::registry& registry, anvil::entity entity)
{
    if (registry.has<anvil::NameComponent>(entity)) {
        return registry.get<anvil::NameComponent>(entity).name;
    }
    return "Entity";
}

bool substring_case_insensitive(std::string_view string, std::string_view substr)
{
    auto it = std::search(
        string.begin(), string.end(),
        substr.begin(), substr.end(),
        [] (char c1, char c2) { return std::toupper(c1) == std::toupper(c2); }
    );
    return it != string.end();
}

}

namespace anvil {

app::app(spkt::window* window)
    : d_window(window)
    , d_asset_manager()
    , d_entity_renderer(&d_asset_manager)
    , d_skybox_renderer()
    , d_skybox({
        "Resources/Textures/Skybox/Skybox_X_Pos.png",
        "Resources/Textures/Skybox/Skybox_X_Neg.png",
        "Resources/Textures/Skybox/Skybox_Y_Pos.png",
        "Resources/Textures/Skybox/Skybox_Y_Neg.png",
        "Resources/Textures/Skybox/Skybox_Z_Pos.png",
        "Resources/Textures/Skybox/Skybox_Z_Neg.png"
    })
    , d_editor_camera(d_window, {0.0, 0.0, 0.0})
    , d_viewport(1280, 720)
    , d_ui(d_window)
{
    d_window->set_cursor_visibility(true);

    d_scene = std::make_shared<anvil::scene>();
    anvil::load_registry_from_file(d_sceneFile, d_scene->registry);
    d_active_scene = d_scene;
}

void app::on_event(spkt::event& event)
{
    using namespace spkt;

    if (auto data = event.get_if<keyboard_pressed_event>()) {
        if (data->key == Keyboard::ESC) {
            if (d_playing_game) {
                d_playing_game = false;
                d_active_scene = d_scene;
                d_window->set_cursor_visibility(true);
            }
            else if (d_selected != anvil::null) {
                d_selected = anvil::null;
            }
            else if (d_window->is_fullscreen()) {
                d_window->set_windowed(1280, 720);
            }
            event.consume();
        } else if (data->key == Keyboard::F11) {
            if (d_window->is_fullscreen()) {
                d_window->set_windowed(1280, 720);
            }
            else {
                d_window->set_fullscreen();
            }
            event.consume();
        }
    }

    d_ui.on_event(event);
    d_active_scene->on_event(event);
    if (!d_playing_game) {
        d_editor_camera.on_event(event);
    }
}

void app::on_update(double dt)
{
    d_ui.on_update(dt);

    if (d_paused) {
        return;
    }

    d_active_scene->on_update(dt);
    if (d_is_viewport_focused && !d_playing_game) {
        d_editor_camera.on_update(dt);
    }
}

void app::on_render()
{
    auto& registry = d_active_scene->registry;

    // If the size of the viewport has changed since the previous frame, recreate
    // the framebuffer.
    if (d_viewport_size != d_viewport.size() && d_viewport_size.x > 0 && d_viewport_size.y > 0) {
        d_viewport.resize(d_viewport_size.x, d_viewport_size.y);
    }

    d_viewport.bind();

    const auto [proj, view] = std::invoke([&] {
        if (d_playing_game) {
            return anvil::get_proj_view_matrices(registry, d_runtime_camera);
        }
        return std::make_pair(d_editor_camera.proj(), d_editor_camera.view());
    });

    anvil::draw_scene(d_entity_renderer, registry, proj, view);
    d_skybox_renderer.draw(d_skybox, proj, view);

    if (d_show_colliders) {
        anvil::draw_colliders(d_collider_renderer, registry, proj, view);
    }

    d_viewport.unbind();

    d_ui.StartFrame();

    ImGui::DockSpaceOverViewport();
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                auto file = spkt::save_file(d_window, "*.yaml");
                if (file.has_value()) {
                    spkt::log::info("Creating {}...", d_sceneFile);
                    d_sceneFile = *file;
                    d_active_scene = d_scene = std::make_shared<anvil::scene>();
                    spkt::log::info("...done!");
                }
            }
            if (ImGui::MenuItem("Open")) {
                auto file = spkt::open_file(d_window, "*.yaml");
                if (file.has_value()) {
                    spkt::log::info("Loading {}...", d_sceneFile);
                    d_sceneFile = *file;
                    d_active_scene = d_scene = std::make_shared<anvil::scene>();
                    anvil::load_registry_from_file(*file, d_scene->registry);
                    spkt::log::info("...done!");
                }
            }

            const auto entity_filter = [](const anvil::registry& reg, anvil::entity entity) {
                return !reg.has<anvil::Runtime>(entity);
            };

            if (ImGui::MenuItem("Save")) {
                spkt::log::info("Saving {}...", d_sceneFile);
                anvil::save_registry_to_file(d_sceneFile, d_scene->registry, entity_filter);
                spkt::log::info("...done!");
            }
            if (ImGui::MenuItem("Save As")) {
                auto file = spkt::save_file(d_window, "*.yaml");
                if (file.has_value()) {
                    spkt::log::info("Saving as {}...", *file);
                    d_sceneFile = *file;
                    anvil::save_registry_to_file(*file, d_scene->registry, entity_filter);
                    spkt::log::info("...done!");
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("Run")) {
                d_active_scene = std::make_shared<anvil::scene>();

                anvil::input_system_init(d_active_scene->registry, d_window);
                anvil::copy_registry(d_scene->registry, d_active_scene->registry);

                d_active_scene->systems = {
                    physics_system,
                    anvil::particle_system,
                    anvil::script_system,
                    anvil::animation_system,
                    anvil::delete_below_50_system,
                    anvil::clear_events_system,
                    anvil::input_system_end
                };

                d_active_scene->event_handlers = {
                    anvil::input_system_on_event
                };

                d_playing_game = true;
                d_runtime_camera = d_active_scene->registry.find<anvil::Camera3DComponent>();
                d_window->set_cursor_visibility(false);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // VIEWPORT
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        d_is_viewport_hovered = ImGui::IsWindowHovered();
        d_is_viewport_focused = ImGui::IsWindowFocused();
        d_ui.BlockEvents(!d_is_viewport_focused || !d_is_viewport_hovered);

        ImVec2 size = ImGui::GetContentRegionAvail();
        d_viewport_size = glm::ivec2{size.x, size.y};

        //auto viewportMouse = ImGuiXtra::GetMousePosWindowCoords();

        spkt::ImGuiXtra::Image(d_viewport.colour_texture());

        if (!is_game_running() && registry.valid(d_selected) && registry.has<anvil::Transform3DComponent>(d_selected)) {
            auto& c = registry.get<anvil::Transform3DComponent>(d_selected);
            auto tr = spkt::make_transform(c.position, c.orientation, c.scale);
            spkt::ImGuiXtra::Guizmo(&tr, view, proj, d_inspector.operation(), d_inspector.mode());
            std::tie(c.position, c.orientation, c.scale) = spkt::decompose(tr).as_tuple();
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();

    // INSPECTOR
    // TODO: Move the static variables from here to the class def
    static bool showInspector = true;
    if (ImGui::Begin("Inspector", &showInspector)) {
        d_inspector.show(*this);
        ImGui::End();
    }

    // EXPLORER
    static std::string search;
    static bool showExplorer = true;
    if (ImGui::Begin("Explorer", &showExplorer)) {
        spkt::ImGuiXtra::TextModifiable(search);
        ImGui::SameLine();
        if (ImGui::Button("X")) {
            search = "";
        }
        if (ImGui::BeginTabBar("##Tabs")) {

            if (ImGui::BeginTabItem("Entities")) {
                ImGui::BeginChild("Entity List");
                int i = 0;
                for (auto entity : registry.all()) {
                    if (substring_case_insensitive(entiy_name(registry, entity), search)) {
                        ImGui::PushID(i);
                        if (ImGui::Selectable(entiy_name(registry, entity).c_str())) {
                            d_selected = entity;
                        }
                        ImGui::PopID();
                    }
                    ++i;
                }
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            const auto hasher = [](const std::string& str) {
                return static_cast<int>(std::hash<std::string>{}(str));
            };

            if (ImGui::BeginTabItem("Materials")) {
                ImGui::BeginChild("Material List");
                for (auto& [file, material] : d_asset_manager.view<spkt::material>()) {
                    ImGui::PushID(hasher(file));
                    if (ImGui::CollapsingHeader(material.name.c_str())) {
                        ImGui::Text(file.c_str());
                        ImGui::Separator();

                        ImGui::PushID(hasher("Albedo"));
                        ImGui::Text("Albedo");
                        ImGui::Checkbox("Use Map", &material.useAlbedoMap);
                        if (material.useAlbedoMap) {
                            material_ui(material.albedoMap);
                        } else {
                            ImGui::ColorEdit3("##Albedo", &material.albedo.x);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        ImGui::PushID(hasher("Normal"));
                        ImGui::Text("Normal");
                        ImGui::Checkbox("Use Map", &material.useNormalMap);
                        if (material.useNormalMap) {
                            material_ui(material.normalMap);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        ImGui::PushID(hasher("Metallic"));
                        ImGui::Text("Metallic");
                        ImGui::Checkbox("Use Map", &material.useMetallicMap);
                        if (material.useMetallicMap) {
                            material_ui(material.metallicMap);
                        } else {
                            ImGui::DragFloat("##Metallic", &material.metallic, 0.01f, 0.0f, 1.0f);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        ImGui::PushID(hasher("Roughness"));
                        ImGui::Text("Roughness");
                        ImGui::Checkbox("Use Map", &material.useRoughnessMap);
                        if (material.useRoughnessMap) {
                            material_ui(material.roughnessMap);
                        } else {
                            ImGui::DragFloat("##Roughness", &material.roughness, 0.01f, 0.0f, 1.0f);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        if (ImGui::Button("Save")) {
                            spkt::material::save(file, material);
                        }
                    }
                    ImGui::PopID();
                }
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    if (ImGui::Begin("Options")) {
        ImGui::Checkbox("Show Colliders", &d_show_colliders);

        anvil::for_each_component([&]<typename T>(anvil::reflcomp<T>&& refl) {
            std::string text = std::format("# {}: {}", refl.name, registry.view<T>().size());
            ImGui::Text(text.c_str());
        });

        ImGui::End();
    }

    d_ui.EndFrame();
}

void app::material_ui(std::string& texture)
{
    if (ImGui::Button("X")) {
        texture = "";
    }
    ImGui::SameLine();
    spkt::ImGuiXtra::File("File", d_window, &texture, "*.png");
}

}