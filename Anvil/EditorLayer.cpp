#include "EditorLayer.h"

namespace Sprocket {

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
    , d_physicsEngine(Maths::vec3(0.0, -9.81, 0.0))
    , d_cameraSystem(core.window->AspectRatio())
    , d_selector(core.window, &d_physicsEngine)
    , d_scene({
        &d_physicsEngine,
        &d_selector,
        &d_cameraSystem,
        //&d_scriptRunner
    })
    , d_serialiser(&d_scene)
    , d_editorCamera(core.window, {0.0, 0.0, 0.0})
    , d_viewport(1280, 720)
    , d_ui(core.window)
{
    d_core.window->SetCursorVisibility(true);
    
    d_scene.OnStartup();
    d_selector.Enable(true);
    d_serialiser.Deserialise("Resources/Anvil.yaml");

    d_scene.Each<CameraComponent>([&](Entity& entity) {
        d_camera = entity;
    });

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
    
    d_ui.OnEvent(event);
    d_scene.OnEvent(event);
    d_editorCamera.OnEvent(event);
}

void EditorLayer::OnUpdate(double dt)
{
    d_ui.OnUpdate(dt);
    
    if (!d_paused) {
        d_scene.OnUpdate(dt);
        d_editorCamera.OnUpdate(dt);
        
        d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};

        d_scene.Each<TransformComponent, PhysicsComponent>([&](Entity& entity) {
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
    //d_entityRenderer.BeginScene(d_camera, d_lights);
    d_entityRenderer.BeginScene(d_editorCamera.Proj(), d_editorCamera.View(), d_lights);
    d_skyboxRenderer.Draw(d_skybox, d_camera);
    d_scene.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        d_entityRenderer.Draw(entity);
    });
    d_viewport.Unbind();

    d_ui.StartFrame();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoMove;
    bool open = true;
    ImGui::SetNextWindowPos({0.0, 0.0});
    ImGui::SetNextWindowSize({(float)d_core.window->Width()/2.0f, (float)d_core.window->Height()/2.0f});
    if (ImGui::Begin("Viewport", &open, window_flags)) {
        ImGui::Image(
            (ImTextureID)(intptr_t)d_viewport.GetTexture().Id(),
            {(float)d_core.window->Width()/2.0f, (float)d_core.window->Height()/2.0f},
            {0, 1}, {1, 0}
        );
        ImGui::End();
    }
    d_ui.EndFrame();    
}

}