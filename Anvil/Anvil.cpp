#include "Anvil.h"

#include <Anvil/Inspector.h>

#include <Sprocket/Scene/Camera.h>
#include <Sprocket/Scene/Loader.h>
#include <Sprocket/Scene/Systems/basic_systems.h>
#include <Sprocket/Scene/Systems/particle_system.h>
#include <Sprocket/Scene/Systems/physics_system.h>
#include <Sprocket/UI/ImGuiXtra.h>
#include <Sprocket/Utility/FileBrowser.h>
#include <Sprocket/Utility/KeyboardCodes.h>
#include <Sprocket/Utility/Log.h>
#include <Sprocket/Utility/Maths.h>
#include <Sprocket/Vendor/imgui/imgui.h>

#include <glm/glm.hpp>

#include <string_view>
#include <ranges>

namespace {

std::string entiy_name(spkt::registry& registry, spkt::entity entity)
{
    if (registry.has<spkt::NameComponent>(entity)) {
        return registry.get<spkt::NameComponent>(entity).name;
    }
    return "Entity";
}

bool SubstringCI(std::string_view string, std::string_view substr) {
    auto it = std::search(
        string.begin(), string.end(),
        substr.begin(), substr.end(),
        [] (char c1, char c2) { return std::toupper(c1) == std::toupper(c2); }
    );
    return it != string.end();
}

}

Anvil::Anvil(spkt::Window* window) 
    : d_window(window)
    , d_asset_manager()
    , d_entity_renderer(&d_asset_manager)
    , d_skybox_renderer(&d_asset_manager)
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
    d_window->SetCursorVisibility(true);

    d_scene = std::make_shared<spkt::Scene>(); 
    spkt::add_singleton(d_scene->registry());   
    spkt::load_registry_from_file(d_sceneFile, d_scene->registry());
    d_activeScene = d_scene;
}

void Anvil::on_event(spkt::event& event)
{
    using namespace spkt;

    if (auto data = event.get_if<KeyboardButtonPressed>()) {
        if (data->key == Keyboard::ESC) {
            if (d_playingGame) {
                d_playingGame = false;
                d_activeScene = d_scene;
                d_window->SetCursorVisibility(true);
            }
            else if (d_selected != spkt::null) {
                d_selected = spkt::null;
            }
            else if (d_window->IsFullscreen()) {
                d_window->SetWindowed(1280, 720);
            }
            event.consume();
        } else if (data->key == Keyboard::F11) {
            if (d_window->IsFullscreen()) {
                d_window->SetWindowed(1280, 720);
            }
            else {
                d_window->SetFullscreen();
            }
            event.consume();
        }
    }

    d_ui.on_event(event);
    d_activeScene->on_event(event);
    if (!d_playingGame) {
        d_editor_camera.on_event(event);
    }
}

void Anvil::on_update(double dt)
{
    auto& registry = d_activeScene->registry();

    d_ui.on_update(dt);

    if (d_paused) {
        return;
    }

    d_activeScene->on_update(dt);

    if (d_is_viewport_focused && !d_playingGame) {
        d_editor_camera.on_update(dt);
    }
}

glm::mat4 Anvil::get_proj_matrix() const
{
    auto& registry = d_activeScene->registry();
    return d_playingGame ? spkt::make_proj(registry, d_runtimeCamera) : d_editor_camera.Proj();
}

glm::mat4 Anvil::get_view_matrix() const
{
    auto& registry = d_activeScene->registry();
    return d_playingGame ? spkt::make_view(registry, d_runtimeCamera) : d_editor_camera.View();
}

void Anvil::on_render()
{
    auto& registry = d_activeScene->registry();

    // If the size of the viewport has changed since the previous frame, recreate
    // the framebuffer.
    if (d_viewport_size != d_viewport.Size() && d_viewport_size.x > 0 && d_viewport_size.y > 0) {
        d_viewport.SetScreenSize(d_viewport_size.x, d_viewport_size.y);
    }

    d_viewport.Bind();

    glm::mat4 proj = get_proj_matrix();
    glm::mat4 view = get_view_matrix();

    d_entity_renderer.Draw(registry, proj, view);
    d_skybox_renderer.Draw(d_skybox, proj, view);
    if (d_showColliders) {
        d_collider_renderer.Draw(registry, proj, view);
    }

    d_viewport.Unbind();


    d_ui.StartFrame();

    ImGui::DockSpaceOverViewport();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                std::string file = spkt::SaveFile(d_window, "*.yaml");
                if (!file.empty()) {
                    spkt::log::info("Creating {}...", d_sceneFile);
                    d_sceneFile = file;
                    d_activeScene = d_scene = std::make_shared<spkt::Scene>();
                    spkt::log::info("...done!");
                }
            }
            if (ImGui::MenuItem("Open")) {
                std::string file = spkt::OpenFile(d_window, "*.yaml");
                if (!file.empty()) {
                    spkt::log::info("Loading {}...", d_sceneFile);
                    d_sceneFile = file;
                    d_activeScene = d_scene = std::make_shared<spkt::Scene>();
                    spkt::load_registry_from_file(file, d_scene->registry());
                    spkt::log::info("...done!");
                }
            }
            if (ImGui::MenuItem("Save")) {
                spkt::log::info("Saving {}...", d_sceneFile);
                spkt::save_registry_to_file(d_sceneFile, d_scene->registry());
                spkt::log::info("...done!");
            }
            if (ImGui::MenuItem("Save As")) {
                std::string file = spkt::SaveFile(d_window, "*.yaml");
                if (!file.empty()) {
                    spkt::log::info("Saving as {}...", file);
                    d_sceneFile = file;
                    spkt::save_registry_to_file(file, d_scene->registry());
                    spkt::log::info("...done!");
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("Run")) {
                d_activeScene = std::make_shared<spkt::Scene>();

                spkt::add_singleton(d_activeScene->registry());
                spkt::copy_registry(d_scene->registry(), d_activeScene->registry());

                d_activeScene->add(spkt::physics_system);
                d_activeScene->add(spkt::particle_system);
                d_activeScene->add(spkt::camera_system);
                d_activeScene->add(spkt::script_system);
                d_activeScene->add(spkt::animation_system);
                d_activeScene->add(spkt::delete_below_50_system);
                d_activeScene->add(spkt::clear_events_system);

                d_playingGame = true;
                d_runtimeCamera = d_activeScene->registry().find<spkt::Camera3DComponent>();
                d_window->SetCursorVisibility(false);
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

        spkt::ImGuiXtra::Image(d_viewport.GetTexture());

        if (!is_game_running() && registry.valid(d_selected) && registry.has<spkt::Transform3DComponent>(d_selected)) {
            auto& c = registry.get<spkt::Transform3DComponent>(d_selected);
            auto tr = spkt::Maths::Transform(c.position, c.orientation, c.scale);
            spkt::ImGuiXtra::Guizmo(&tr, view, proj, d_inspector.Operation(), d_inspector.Mode());
            spkt::Maths::Decompose(tr, &c.position, &c.orientation, &c.scale);
        }
        ImGui::End();
    }
    ImGui::PopStyleVar();

    // INSPECTOR
    static bool showInspector = true;
    if (ImGui::Begin("Inspector", &showInspector)) {
        d_inspector.Show(*this);
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
                    if (SubstringCI(entiy_name(registry, entity), search)) {
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
                for (auto& [file, material] : d_asset_manager.Materials()) {
                    ImGui::PushID(hasher(material->file));
                    if (ImGui::CollapsingHeader(material->name.c_str())) {
                        ImGui::Text(file.c_str());
                        ImGui::Separator();

                        ImGui::PushID(hasher("Albedo"));
                        ImGui::Text("Albedo");
                        ImGui::Checkbox("Use Map", &material->useAlbedoMap);
                        if (material->useAlbedoMap) {
                            material_ui(material->albedoMap);
                        } else {
                            ImGui::ColorEdit3("##Albedo", &material->albedo.x);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        ImGui::PushID(hasher("Normal"));
                        ImGui::Text("Normal");
                        ImGui::Checkbox("Use Map", &material->useNormalMap);
                        if (material->useNormalMap) {
                            material_ui(material->normalMap);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        ImGui::PushID(hasher("Metallic"));
                        ImGui::Text("Metallic");
                        ImGui::Checkbox("Use Map", &material->useMetallicMap);
                        if (material->useMetallicMap) {
                            material_ui(material->metallicMap);
                        } else {
                            ImGui::DragFloat("##Metallic", &material->metallic, 0.01f, 0.0f, 1.0f);
                        }
                        ImGui::PopID();
                        ImGui::Separator();
                        
                        ImGui::PushID(hasher("Roughness"));
                        ImGui::Text("Roughness");
                        ImGui::Checkbox("Use Map", &material->useRoughnessMap);
                        if (material->useRoughnessMap) {
                            material_ui(material->roughnessMap);
                        } else {
                            ImGui::DragFloat("##Roughness", &material->roughness, 0.01f, 0.0f, 1.0f);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        if (ImGui::Button("Save")) {
                            material->Save();
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
        ImGui::Checkbox("Show Colliders", &d_showColliders);
        ImGui::End();
    }

    d_ui.EndFrame();    
}

void Anvil::material_ui(std::string& texture)
{
    if (ImGui::Button("X")) {
        texture = "";
    }
    ImGui::SameLine();
    spkt::ImGuiXtra::File("File", d_window, &texture, "*.png");
}