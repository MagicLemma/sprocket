#include "EditorLayer.h"
#include "FileBrowser.h"
#include "ImGuiHelpers.cpp"

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

        d_activeScene->Each<TransformComponent, RigidBody3DComponent>([&](Entity& entity) {
            auto& transform = entity.Get<TransformComponent>();
            auto& physics = entity.Get<RigidBody3DComponent>();
            
            if (entity.Has<CameraComponent>() && transform.position.y < -2) {
                transform.position = {0, 3, 0};
                physics.velocity = {0, 0, 0};
            }
            if (transform.position.y < -50) {
                entity.Kill();
            }
        });
    }

    d_entityRenderer.RenderColliders(!d_playingGame);

    d_viewport.Bind();
    if (d_playingGame) {
        d_entityRenderer.BeginScene(d_runtimeCamera, d_lights, *d_activeScene);
        d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
    }
    else {
        d_entityRenderer.BeginScene(d_editorCamera.Proj(), d_editorCamera.View(), d_lights, *d_activeScene);
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
        if (!d_selected.Null()) {
            EntityInspector(d_selected);
        }
        else {
            if (ImGui::Button("New Entity")) {
                auto e = d_scene->NewEntity();
                d_selected = e;
            }
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
            ImGuiXtra::TextModifiable(c.name);
            if (ImGui::Button("Delete")) { entity.Remove<NameComponent>(); }
        }
    }
    if (entity.Has<TransformComponent>()) {
        auto& c = entity.Get<TransformComponent>();
        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::DragFloat3("Transform", &c.position.x, 0.1f);
            ImGui::DragFloat4("Orientation", &c.orientation.x, 0.1f);
            if (ImGui::RadioButton("Translate", mode == DevUI::GizmoMode::TRANSLATION)) {
                mode = DevUI::GizmoMode::TRANSLATION;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", mode == DevUI::GizmoMode::ROTATION)) {
                mode = DevUI::GizmoMode::ROTATION;
            }

            if (ImGui::RadioButton("World", coords == DevUI::GizmoCoords::WORLD)) {
                coords = DevUI::GizmoCoords::WORLD;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Local", coords == DevUI::GizmoCoords::LOCAL)) {
                coords = DevUI::GizmoCoords::LOCAL;
            }
            if (ImGui::Button("Delete")) {
                entity.Remove<TransformComponent>();
            }
        }

        if (!d_playingGame) {
            auto tr = Maths::Transform(c.position, c.orientation);
            d_ui.Gizmo(&tr, d_editorCamera.View(), d_editorCamera.Proj(), mode, coords);
            c.position = Maths::GetTranslation(tr);
            c.orientation = Maths::ToQuat(Maths::mat3(tr));
            Maths::Normalise(c.orientation);
        }
    }
    if (entity.Has<ModelComponent>()) {
        if (ImGui::CollapsingHeader("Model")) {
            auto& c = entity.Get<ModelComponent>();
            ImGuiXtra::File("Model", d_core.window, &c.model, "*.obj");
            ImGui::DragFloat("Scale", &c.scale, 0.1f);
            ImGuiXtra::File("Texture", d_core.window, &c.texture, "*.png");
            ImGui::DragFloat("Shine Damper", &c.shineDamper, 0.1f);
            ImGui::DragFloat("Reflectivity", &c.reflectivity, 0.1f);
            if (ImGui::Button("Delete")) { entity.Remove<ModelComponent>(); }
        }
    }
    if (entity.Has<RigidBody3DComponent>()) {
        if (ImGui::CollapsingHeader("RigidBody3D")) {
            auto& c = entity.Get<RigidBody3DComponent>();
            ImGui::DragFloat3("Velocity", &c.velocity.x);
            ImGui::Checkbox("Gravity", &c.gravity);
            ImGui::Checkbox("Frozen", &c.frozen);
            ImGui::SliderFloat("Bounciness", &c.bounciness, 0.0, 1.0);
            ImGui::SliderFloat("Friction Coefficient", &c.frictionCoefficient, 0.0, 1.0);
            ImGui::SliderFloat("Rolling Resistance", &c.rollingResistance, 0.0, 1.0);
            if (ImGui::Button("Delete")) { entity.Remove<RigidBody3DComponent>(); }
        }
    }
    if (entity.Has<BoxCollider3DComponent>()) {
        if (ImGui::CollapsingHeader("BoxCollider3D")) {
            auto& c = entity.Get<BoxCollider3DComponent>();
            ImGui::DragFloat3("Velocity", &c.halfExtents.x, 0.01f);
        }
    }
    if (entity.Has<SphereCollider3DComponent>()) {
        if (ImGui::CollapsingHeader("SphereCollider3D")) {
            auto& c = entity.Get<SphereCollider3DComponent>();
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
        }
    }
    if (entity.Has<CapsuleCollider3DComponent>()) {
        if (ImGui::CollapsingHeader("CapsuleCollider3D")) {
            auto& c = entity.Get<CapsuleCollider3DComponent>();
            ImGui::DragFloat("Radius", &c.radius, 0.01f);
            ImGui::DragFloat("Height", &c.height, 0.01f);
        }
    }
    if (entity.Has<ScriptComponent>()) {
        if (ImGui::CollapsingHeader("Script")) {
            auto& c = entity.Get<ScriptComponent>();
            ImGuiXtra::File("Script", d_core.window, &c.script, "*.lua");
            ImGui::Checkbox("Active", &c.active);
            if (ImGui::Button("Delete")) { entity.Remove<ScriptComponent>(); }
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
    if (entity.Has<LightComponent>()) {
        if (ImGui::CollapsingHeader("Light")) {
            auto& c = entity.Get<LightComponent>();
            ImGui::ColorPicker3("Colour", &c.colour.r);
            ImGui::DragFloat3("Attenuation", &c.attenuation.x);
            ImGui::DragFloat("Brightness", &c.brightness);
            if (ImGui::Button("Delete")) { entity.Remove<LightComponent>(); }
        }
    }
    ImGui::Separator();
    if (ImGui::BeginMenu("Add Component")) {
        if (!entity.Has<NameComponent>() && ImGui::MenuItem("Name")) {
            NameComponent c;
            entity.Add(c);
        }
        if (!entity.Has<TransformComponent>() && ImGui::MenuItem("Transform")) {
            TransformComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ModelComponent>() && ImGui::MenuItem("Model")) {
            ModelComponent c;
            entity.Add(c);
        }
        if (!entity.Has<ScriptComponent>() && ImGui::MenuItem("Script")) {
            ScriptComponent c;
            entity.Add(c);
        }
        if (!entity.Has<LightComponent>() && ImGui::MenuItem("Light")) {
            LightComponent c;
            entity.Add(c);
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Delete Entity")) {
        entity.Kill();
        d_selected = Entity();
    }
}

}