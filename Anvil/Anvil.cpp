#include "Anvil.h"
#include "Inspector.h"
#include "FileBrowser.h"
#include "ImGuiXtra.cpp"

#include <fmt/core.h>
#include <string_view>

namespace Sprocket {
namespace {

std::string Name(const ecs::Entity& entity)
{
    if (entity.has<NameComponent>()) {
        return entity.get<NameComponent>().name;
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

Anvil::Anvil(Window* window) 
    : d_window(window)
    , d_assetManager()
    , d_entityRenderer(&d_assetManager)
    , d_skyboxRenderer(&d_assetManager)
    , d_skybox({
        "Resources/Textures/Skybox/Skybox_X_Pos.png",
        "Resources/Textures/Skybox/Skybox_X_Neg.png",
        "Resources/Textures/Skybox/Skybox_Y_Pos.png",
        "Resources/Textures/Skybox/Skybox_Y_Neg.png",
        "Resources/Textures/Skybox/Skybox_Z_Pos.png",
        "Resources/Textures/Skybox/Skybox_Z_Neg.png"
    })
    , d_editorCamera(d_window, {0.0, 0.0, 0.0})
    , d_viewport(1280, 720)
    , d_ui(d_window)
{
    d_window->SetCursorVisibility(true);

    d_scene = std::make_shared<Scene>();    
    d_scene->Load(d_sceneFile);

    d_runtimeCamera = d_scene->Entities().find([](ecs::Entity entity) {
        return entity.has<Camera3DComponent>();
    });

    d_activeScene = d_scene;
}

void Anvil::OnEvent(ev::Event& event)
{
    if (auto data = event.get_if<ev::KeyboardButtonPressed>()) {
        if (data->key == Keyboard::ESC) {
            if (d_playingGame) {
                d_playingGame = false;
                d_activeScene = d_scene;
                d_window->SetCursorVisibility(true);
            }
            else if (d_selected != ecs::Null) {
                d_selected = ecs::Null;
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

    d_ui.OnEvent(event);
    d_activeScene->OnEvent(event);
    if (!d_playingGame) {
        d_editorCamera.OnEvent(event);
    }
}

void Anvil::OnUpdate(double dt)
{
    d_ui.OnUpdate(dt);
    //d_window->SetWindowName(fmt::format("Anvil: {}", d_sceneFile));

    // Create the Shadow Map
    //float lambda = 5.0f; // TODO: Calculate the floor intersection point
    //glm::vec3 target = d_camera.Get<TransformComponent>().position + lambda * Maths::Forwards(d_camera.Get<TransformComponent>().orientation);
    //d_shadowMap.Draw(sun, target, *d_scene);
    //d_entityRenderer.EnableShadows(d_shadowMap);

    if (d_paused) {
        return;
    }

    d_activeScene->OnUpdate(dt);
    d_particleManager.OnUpdate(dt);

    if (d_isViewportFocused && !d_playingGame) {
        d_editorCamera.OnUpdate(dt);
    }
    
    std::vector<ecs::Entity> toDelete;
    for (auto entity : d_activeScene->Entities().view<Transform3DComponent>()) {
        auto& transform = entity.get<Transform3DComponent>();
        if (transform.position.y < -50) {
            toDelete.push_back(entity);
        }
    }
    for (auto entity : toDelete) {
        entity.destroy();
    }
}

glm::mat4 Anvil::GetProjMatrix() const
{
    return d_playingGame ? MakeProj(d_runtimeCamera) : d_editorCamera.Proj();
}

glm::mat4 Anvil::GetViewMatrix() const
{
    return d_playingGame ? MakeView(d_runtimeCamera) : d_editorCamera.View();
}

void Anvil::OnRender()
{
    // If the size of the viewport has changed since the previous frame, recreate
    // the framebuffer.
    if (d_viewportSize != d_viewport.Size() && d_viewportSize.x > 0 && d_viewportSize.y > 0) {
        d_viewport.SetScreenSize(d_viewportSize.x, d_viewportSize.y);
    }

    d_entityRenderer.EnableParticles(&d_particleManager);
    d_viewport.Bind();

    glm::mat4 proj = GetProjMatrix();
    glm::mat4 view = GetViewMatrix();

    d_entityRenderer.Draw(proj, view, *d_activeScene);
    d_skyboxRenderer.Draw(d_skybox, proj, view);
    if (d_showColliders) {
        d_colliderRenderer.Draw(proj, view, *d_activeScene);
    }

    d_viewport.Unbind();


    d_ui.StartFrame();

    ImGui::DockSpaceOverViewport();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                std::string file = SaveFile(d_window, "*.yaml");
                if (!file.empty()) {
                    log::info("Creating {}...", d_sceneFile);
                    d_sceneFile = file;
                    d_scene->Clear();
                    log::info("...done!");
                }
            }
            if (ImGui::MenuItem("Open")) {
                std::string file = OpenFile(d_window, "*.yaml");
                if (!file.empty()) {
                    log::info("Loading {}...", d_sceneFile);
                    d_sceneFile = file;
                    d_scene->Clear();
                    Loader::Load(file, &d_scene->Entities());
                    log::info("...done!");
                }
            }
            if (ImGui::MenuItem("Save")) {
                log::info("Saving {}...", d_sceneFile);
                Loader::Save(d_sceneFile, &d_scene->Entities());
                log::info("...done!");
            }
            if (ImGui::MenuItem("Save As")) {
                std::string file = SaveFile(d_window, "*.yaml");
                if (!file.empty()) {
                    log::info("Saving as {}...", file);
                    d_sceneFile = file;
                    Loader::Save(file, &d_scene->Entities());
                    log::info("...done!");
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("Run")) {
                d_activeScene = std::make_shared<Scene>(); 
                d_activeScene->Add<PhysicsEngine3D>();
                d_activeScene->Add<CameraSystem>(d_window->AspectRatio());
                d_activeScene->Add<ScriptRunner>(d_window);
                d_activeScene->Add<ParticleSystem>(&d_particleManager);
                d_activeScene->Add<AnimationSystem>();
                Loader::Copy(&d_scene->Entities(), &d_activeScene->Entities());

                d_playingGame = true;
                d_runtimeCamera = d_activeScene->Entities().find<Camera3DComponent>();
                d_window->SetCursorVisibility(false);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // VIEWPORT
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        d_isViewportHovered = ImGui::IsWindowHovered();
        d_isViewportFocused = ImGui::IsWindowFocused();
        d_ui.BlockEvents(!d_isViewportFocused || !d_isViewportHovered);

        ImVec2 size = ImGui::GetContentRegionAvail();
        d_viewportSize = glm::ivec2{size.x, size.y};

        //auto viewportMouse = ImGuiXtra::GetMousePosWindowCoords();

        ImGuiXtra::Image(d_viewport.GetTexture());

        if (!IsGameRunning() && d_selected.valid() && d_selected.has<Transform3DComponent>()) {
            auto& c = d_selected.get<Transform3DComponent>();
            auto tr = Maths::Transform(c.position, c.orientation, c.scale);
            ImGuiXtra::Guizmo(&tr, view, proj, d_inspector.Operation(), d_inspector.Mode());
            Maths::Decompose(tr, &c.position, &c.orientation, &c.scale);
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
        ImGuiXtra::TextModifiable(search);
        ImGui::SameLine();
        if (ImGui::Button("X")) {
            search = "";
        }
        if (ImGui::BeginTabBar("##Tabs")) {
            
            if (ImGui::BeginTabItem("Entities")) {
                ImGui::BeginChild("Entity List");
                int i = 0;
                for (auto entity : d_scene->Entities().all()) {
                    if (SubstringCI(Name(entity), search)) {
                        ImGui::PushID(i);
                        if (ImGui::Selectable(Name(entity).c_str())) {
                            d_selected = entity;
                        }
                        ImGui::PopID();
                    }
                    ++i;
                }
                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            std::hash<std::string> hasher;
            if (ImGui::BeginTabItem("Materials")) {
                ImGui::BeginChild("Material List");
                for (auto& [file, material] : d_assetManager.Materials()) {
                    ImGui::PushID(hasher(material->file));
                    if (ImGui::CollapsingHeader(material->name.c_str())) {
                        ImGui::Text(file.c_str());
                        ImGui::Separator();

                        ImGui::PushID(hasher("Albedo"));
                        ImGui::Text("Albedo");
                        ImGui::Checkbox("Use Map", &material->useAlbedoMap);
                        if (material->useAlbedoMap) {
                            MaterialUI(material->albedoMap);
                        } else {
                            ImGui::ColorEdit3("##Albedo", &material->albedo.x);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        ImGui::PushID(hasher("Normal"));
                        ImGui::Text("Normal");
                        ImGui::Checkbox("Use Map", &material->useNormalMap);
                        if (material->useNormalMap) {
                            MaterialUI(material->normalMap);
                        }
                        ImGui::PopID();
                        ImGui::Separator();

                        ImGui::PushID(hasher("Metallic"));
                        ImGui::Text("Metallic");
                        ImGui::Checkbox("Use Map", &material->useMetallicMap);
                        if (material->useMetallicMap) {
                            MaterialUI(material->metallicMap);
                        } else {
                            ImGui::DragFloat("##Metallic", &material->metallic, 0.01f, 0.0f, 1.0f);
                        }
                        ImGui::PopID();
                        ImGui::Separator();
                        
                        ImGui::PushID(hasher("Roughness"));
                        ImGui::Text("Roughness");
                        ImGui::Checkbox("Use Map", &material->useRoughnessMap);
                        if (material->useRoughnessMap) {
                            MaterialUI(material->roughnessMap);
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

void Anvil::MaterialUI(std::string& texture)
{
    if (ImGui::Button("X")) {
        texture = "";
    }
    ImGui::SameLine();
    ImGuiXtra::File("File", d_window, &texture, "*.png");
}

}