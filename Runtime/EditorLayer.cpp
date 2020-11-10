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

bool SubstringCI(const std::string& string, const std::string& substr) {
    auto it = std::search(
        string.begin(), string.end(),
        substr.begin(), substr.end(),
        [] (char c1, char c2) { return std::toupper(c1) == std::toupper(c2); }
    );
    return it != string.end();
}

}

EditorLayer::EditorLayer(const CoreSystems& core) 
    : d_core(core)
    , d_entityRenderer(core.assetManager)
    , d_skyboxRenderer(core.assetManager)
    , d_skybox({
        "Resources/Textures/Skybox/Skybox_X_Pos.png",
        "Resources/Textures/Skybox/Skybox_X_Neg.png",
        "Resources/Textures/Skybox/Skybox_Y_Pos.png",
        "Resources/Textures/Skybox/Skybox_Y_Neg.png",
        "Resources/Textures/Skybox/Skybox_Z_Pos.png",
        "Resources/Textures/Skybox/Skybox_Z_Neg.png"
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
        
        d_activeScene->Each<TransformComponent>([&](Entity& entity) {
            auto& transform = entity.Get<TransformComponent>();
            if (transform.position.y < -50) {
                entity.Kill();
            }
        });
    }

    d_entityRenderer.EnableParticles(&d_particleManager);

    if (d_playingGame) {
        d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
        d_entityRenderer.Draw(d_runtimeCamera, *d_activeScene);
        if (d_showColliders) {
            d_colliderRenderer.Draw(d_runtimeCamera, *d_activeScene);
        }
    }
    else {
        d_skyboxRenderer.Draw(d_skybox, d_editorCamera.Proj(), d_editorCamera.View());
        d_entityRenderer.Draw(d_editorCamera.Proj(), d_editorCamera.View(), *d_activeScene);
        if (d_showColliders) {
            d_colliderRenderer.Draw(d_editorCamera.Proj(), d_editorCamera.View(), *d_activeScene);
        }
    }

    d_ui.StartFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("Run")) {
                auto runningScene = std::make_shared<Scene>();
                Loader::Copy(d_scene, runningScene);
                d_activeScene = runningScene;
                
                d_activeScene->AddSystem(std::make_shared<PhysicsEngine>(Maths::vec3{0.0, -9.81, 0.0}));
                d_activeScene->AddSystem(std::make_shared<CameraSystem>(d_core.window->AspectRatio()));
                d_activeScene->AddSystem(std::make_shared<ScriptRunner>());
                d_activeScene->AddSystem(std::make_shared<ParticleSystem>(&d_particleManager));
                d_activeScene->AddSystem(std::make_shared<AnimationSystem>());

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

    d_ui.EndFrame();    
}

void EditorLayer::MaterialUI(std::string& texture)
{
    if (ImGui::Button("X")) {
        texture = "";
    }
    ImGui::SameLine();
    ImGuiXtra::File("File", d_core.window, &texture, "*.png");
}

}