#include "EditorLayer.h"

namespace Sprocket {
namespace {

std::string Name(const Entity& entity)
{
    if (entity.Has<NameComponent>()) {
        return entity.Get<NameComponent>().name;
    }
    return "Entity";
}

}

EditorLayer::EditorLayer(const CoreSystems& core) 
    : Layer(core)
    , d_entityRenderer(core.window, core.modelManager, core.textureManager)
    , d_skyboxRenderer(core.window)
    , d_skybox({
        ModelManager::LoadModel("Resources/Models/Skybox.obj"),
        CubeMap({
            "Resources/Textures/Skybox/Skybox_X_Pos.png",
            "Resources/Textures/Skybox/Skybox_X_Neg.png",
            "Resources/Textures/Skybox/Skybox_Y_Pos.png",
            "Resources/Textures/Skybox/Skybox_Y_Neg.png",
            "Resources/Textures/Skybox/Skybox_Z_Pos.png",
            "Resources/Textures/Skybox/Skybox_Z_Neg.png"
        })
    })
    , d_editorCamera(core.window, {0.0, 0.0, 0.0})
    , d_viewport(1280, 720)
    , d_ui(core.window)
{
    d_core.window->SetCursorVisibility(true);

    d_scene = std::make_shared<Scene>();    
    Loader::Load(d_sceneFile, d_scene);

    d_scene->OnStartup();

    d_scene->Each<CameraComponent>([&](Entity& entity) {
        d_runtimeCamera = entity;
    });

    d_activeScene = d_scene;
    
    d_lights.points.push_back({{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{-7.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{40.0, 20.0, 0.0}, {0.8f, 0.8f, 0.8f}, {1.0f, 0.0f, 0.0f}});

    d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};
    d_lights.sun.colour = {1.0, 1.0, 1.0};
    d_lights.sun.brightness = 0.2f;
}

void EditorLayer::OnEvent(Event& event)
{
    if (auto e = event.As<WindowResizeEvent>()) {
        d_viewport.SetScreenSize(e->Width(), e->Height());
    }

    if (auto e = event.As<KeyboardButtonPressedEvent>()) {
        if (e->Key() == Keyboard::ESC) {
            if (d_playingGame) {
                d_playingGame = false;
                d_activeScene = d_scene;
                d_core.window->SetCursorVisibility(true);
            }
            else {
                d_selected = Entity();
            }
            e->Consume();
        }
    }

    d_ui.OnEvent(event);
    d_activeScene->OnEvent(event);
    if (!d_playingGame) {
        d_editorCamera.OnEvent(event);
    }
}

void EditorLayer::OnUpdate(double dt)
{
    d_ui.OnUpdate(dt);

    std::string windowName = "Anvil: " + d_sceneFile;
    d_core.window->SetWindowName(windowName);
    
    if (!d_paused) {
        d_activeScene->OnUpdate(dt);

        if (d_isViewportFocused && !d_playingGame) {
            d_editorCamera.OnUpdate(dt);
        }
        
        d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};

        d_activeScene->Each<TransformComponent, PhysicsComponent>([&](Entity& entity) {
            auto& transform = entity.Get<TransformComponent>();
            auto& physics = entity.Get<PhysicsComponent>();
            
            if (entity.Has<CameraComponent>() && transform.position.y < -2) {
                transform.position = {0, 3, 0};
                physics.velocity = {0, 0, 0};
            }
            if (transform.position.y < -50) {
                entity.Kill();
            }
        });
    }

    d_viewport.Bind();
    if (d_playingGame) {
        d_entityRenderer.BeginScene(d_runtimeCamera, d_lights);
        d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
    }
    else {
        d_entityRenderer.BeginScene(d_editorCamera.Proj(), d_editorCamera.View(), d_lights);
        d_skyboxRenderer.Draw(d_skybox, d_editorCamera.Proj(), d_editorCamera.View());
    }

    d_activeScene->Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        d_entityRenderer.Draw(entity);
    });
    d_viewport.Unbind();

    d_ui.StartFrame();

    auto& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;

    ImGuiWindowFlags flags = 
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoNavInputs |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize;
    if (d_playingGame) {
        flags |= ImGuiWindowFlags_NoDecoration;
    }

    bool open = true;
    float menuBarHeight = 19.0f;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
                std::string file = SaveFile(d_core.window, "*.yaml");
                if (!file.empty()) {
                    SPKT_LOG_INFO("Creating {}...", d_sceneFile);
                    d_sceneFile = file;
                    d_scene->Clear();
                    SPKT_LOG_INFO("...done!");
                }
            }
            if (ImGui::MenuItem("Open")) {
                std::string file = OpenFile(d_core.window, "*.yaml");
                if (!file.empty()) {
                    SPKT_LOG_INFO("Loading {}...", d_sceneFile);
                    d_sceneFile = file;
                    Loader::Load(file, d_scene);
                    SPKT_LOG_INFO("...done!");
                }
            }
            if (ImGui::MenuItem("Save")) {
                SPKT_LOG_INFO("Saving {}...", d_sceneFile);
                Loader::Save(d_sceneFile, d_scene);
                SPKT_LOG_INFO("...done!");
            }
            if (ImGui::MenuItem("Save As")) {
                std::string file = SaveFile(d_core.window, "*.yaml");
                if (!file.empty()) {
                    SPKT_LOG_INFO("Saving as {}...", file);
                    d_sceneFile = file;
                    Loader::Save(file, d_scene);
                    SPKT_LOG_INFO("...done!");
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("Run")) {
                auto runningScene = std::make_shared<Scene>();
                Loader::Copy(d_scene, runningScene);
                d_activeScene = runningScene;
                
                d_activeScene->AddSystem(std::make_shared<PhysicsEngine>(Maths::vec3{0.0, -9.81, 0.0}));
                d_activeScene->AddSystem(std::make_shared<CameraSystem>(d_core.window->AspectRatio()));
                d_activeScene->AddSystem(std::make_shared<ScriptRunner>());

                d_activeScene->OnStartup();
                d_playingGame = true;

                d_activeScene->Each<Sprocket::CameraComponent>([&](Entity& entity) {
                    d_runtimeCamera = entity;
                });
                d_core.window->SetCursorVisibility(false);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    float w = (float)d_core.window->Width();
    float h = (float)d_core.window->Height();

    ImGui::SetNextWindowPos({0.0, menuBarHeight});
    ImGui::SetNextWindowSize({0.8f * w, 0.8f * h});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    if (ImGui::Begin("Viewport", &open, flags | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        d_isViewportHovered = ImGui::IsWindowHovered();
        d_isViewportFocused = ImGui::IsWindowFocused();
        d_ui.BlockEvents(!d_isViewportFocused || !d_isViewportHovered);
        ImGui::Image(
            (ImTextureID)(intptr_t)d_viewport.GetTexture().Id(),
            {0.8f * w, 0.8f * h},
            {0, 1}, {1, 0}
        );
        float rw = ImGui::GetWindowWidth();
        float rh = ImGui::GetWindowHeight();
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
        ImGui::End();
    }
    ImGui::PopStyleVar();

    ImGui::SetNextWindowPos({0.8f * w, menuBarHeight});
    ImGui::SetNextWindowSize({0.2f * w, (h - menuBarHeight)/2.0f});
    if (ImGui::Begin("Entities", &open, flags)) {
        d_scene->All([&](Entity& entity) {
            AddEntityToList(entity);
        });
        ImGui::End();
    }

    ImGui::SetNextWindowPos({0.8f * w, menuBarHeight + (h - menuBarHeight)/2.0f});
    ImGui::SetNextWindowSize({0.2f * w, (h - menuBarHeight)/2.0f});
    if (ImGui::Begin("Inspector", &open, flags)) {
        if (!d_selected.Null()) {
            EntityInspector(d_selected);
        }
        else {
            ImGui::Text("No Entity Selected");
        }
        ImGui::End();
    }

    ImGui::SetNextWindowPos({0.0, 0.8f * h + menuBarHeight});
    ImGui::SetNextWindowSize({0.8f * w, h - menuBarHeight - 0.8f * h});
    if (ImGui::Begin("BottomPanel", &open, flags)) {
        ImGui::End();
    }

    d_ui.EndFrame();    
}

void EditorLayer::AddEntityToList(const Entity& entity)
{
    ImGui::PushID(entity.Id());
    if (ImGui::TreeNode(Name(entity).c_str())) {
        if (ImGui::Button("Select")) {
            d_selected = entity;
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void EditorLayer::EntityInspector(Entity& entity)
{
    static DevUI::GizmoCoords coords = DevUI::GizmoCoords::WORLD;
    static DevUI::GizmoMode mode = DevUI::GizmoMode::ROTATION;

    if (entity.Has<NameComponent>()) {
        if (ImGui::CollapsingHeader("Name")) {
            auto& c = entity.Get<NameComponent>();
            if (ImGui::Button("Delete")) {
                entity.Remove<NameComponent>();
            }
        }
    }
    if (entity.Has<TransformComponent>()) {
        auto& c = entity.Get<TransformComponent>();
        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::DragFloat3("Transform", &c.position.x, 0.1f);
            ImGui::DragFloat4("Orientation", &c.orientation.x, 0.1f);
            if (ImGui::Button("Delete")) {
                entity.Remove<TransformComponent>();
            }
        }

        auto tr = Maths::Transform(c.position, c.orientation);
        d_ui.Gizmo(&tr, d_editorCamera.View(), d_editorCamera.Proj(), mode, coords);
        c.position = Maths::GetTranslation(tr);
        c.orientation = Maths::ToQuat(Maths::mat3(tr));
        Maths::Normalise(c.orientation);
    }
    if (entity.Has<ModelComponent>()) {
        if (ImGui::CollapsingHeader("Model")) {
            auto& c = entity.Get<ModelComponent>();
            ImGui::DragFloat("Scale", &c.scale, 0.1f);
            ImGui::DragFloat("Shine Damper", &c.shineDamper, 0.1f);
            ImGui::DragFloat("Reflectivity", &c.reflectivity, 0.1f);
            if (ImGui::Button("Delete")) {
                entity.Remove<ModelComponent>();
            }
        }
    }
    if (entity.Has<PhysicsComponent>()) {
        if (ImGui::CollapsingHeader("Physics")) {
            auto& c = entity.Get<PhysicsComponent>();
            if (ImGui::Button("Delete")) {
                entity.Remove<PhysicsComponent>();
            }
        }
    }
    if (entity.Has<ScriptComponent>()) {
        if (ImGui::CollapsingHeader("Script")) {
            auto& c = entity.Get<ScriptComponent>();
            if (ImGui::Button("Delete")) {
                entity.Remove<ScriptComponent>();
            }
        }
    }
    if (entity.Has<CameraComponent>()) {
        if (ImGui::CollapsingHeader("Camera")) {
            auto& c = entity.Get<CameraComponent>();
            if (ImGui::Button("Delete")) {
                entity.Remove<CameraComponent>();
            }
        }
    }
    if (entity.Has<SelectComponent>()) {
        if (ImGui::CollapsingHeader("Select")) {
            auto& c = entity.Get<SelectComponent>();
            if (ImGui::Button("Delete")) {
                entity.Remove<SelectComponent>();
            }
        }
    }
    if (entity.Has<PathComponent>()) {
        if (ImGui::CollapsingHeader("Path")) {
            auto& c = entity.Get<PathComponent>();
            if (ImGui::Button("Delete")) {
                entity.Remove<PathComponent>();
            }
        }
    }
    ImGui::Separator();
    if (ImGui::Button("Delete Entity")) {
        entity.Kill();
        d_selected = Entity();
    }
}

}