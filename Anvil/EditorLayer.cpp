#include "EditorLayer.h"
#include "Inspector.h"
#include "FileBrowser.h"
#include "ImGuiXtra.cpp"

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
    , d_entityRenderer(core.modelManager, core.textureManager)
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
        d_particleManager.OnUpdate(dt);

        if (d_isViewportFocused && !d_playingGame) {
            d_editorCamera.OnUpdate(dt);
        }
        
        d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};

        d_activeScene->Each<TransformComponent>([&](Entity& entity) {
            auto& transform = entity.Get<TransformComponent>();
            if (transform.position.y < -50) {
                entity.Kill();
            }
        });
    }

    d_entityRenderer.EnableParticles(&d_particleManager);

    d_viewport.Bind();
    if (d_playingGame) {
        d_entityRenderer.Draw(d_runtimeCamera, d_lights, *d_activeScene);
        d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
        if (d_showColliders) {
            d_colliderRenderer.Draw(d_runtimeCamera, *d_activeScene);
        }
    }
    else {
        d_entityRenderer.Draw(d_editorCamera.Proj(), d_editorCamera.View(), d_lights, *d_activeScene);
        d_skyboxRenderer.Draw(d_skybox, d_editorCamera.Proj(), d_editorCamera.View());
        if (d_showColliders) {
            d_colliderRenderer.Draw(d_editorCamera.Proj(), d_editorCamera.View(), *d_activeScene);
        }
    }


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
                d_activeScene->AddSystem(std::make_shared<ParticleSystem>(&d_particleManager));

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
    if (ImGui::Begin("Viewport", &open, flags | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        d_isViewportHovered = ImGui::IsWindowHovered();
        d_isViewportFocused = ImGui::IsWindowFocused();
        d_ui.BlockEvents(!d_isViewportFocused || !d_isViewportHovered);
        ImGuiXtra::Image(d_viewport.GetTexture(), {0.8f * w, 0.8f * h});
        ImGuiXtra::SetGuizmo();
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
        ShowInspector(*this);
        ImGui::End();
    }

    ImGui::SetNextWindowPos({0.0, 0.8f * h + menuBarHeight});
    ImGui::SetNextWindowSize({0.8f * w, h - menuBarHeight - 0.8f * h});
    if (ImGui::Begin("BottomPanel", &open, flags)) {
        ImGui::Checkbox("Show Colliders", &d_showColliders);
        auto& sun = d_activeScene->GetSun();
        ImGui::DragFloat3("Sun Direction", &sun.direction.x);
        Maths::Normalise(sun.direction);
        ImGui::ColorEdit3("Sun Colour", &sun.colour.x);
        ImGui::DragFloat("Sun Brightness", &sun.brightness, 0.01f);
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

}