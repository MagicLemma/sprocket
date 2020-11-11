#include "EditorLayer.h"

namespace Sprocket {

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
    , d_console(core.window)
{
    d_core.window->SetCursorVisibility(true);
    d_entityRenderer.EnableParticles(&d_particleManager);

    d_scene = std::make_shared<Scene>();    
    d_scene->AddSystem(std::make_shared<PhysicsEngine>(Maths::vec3{0.0, -9.81, 0.0}));
    d_scene->AddSystem(std::make_shared<CameraSystem>(d_core.window->AspectRatio()));
    d_scene->AddSystem(std::make_shared<ScriptRunner>());
    d_scene->AddSystem(std::make_shared<ParticleSystem>(&d_particleManager));
    d_scene->AddSystem(std::make_shared<AnimationSystem>());
    Loader::Load("Resources/Anvil.yaml", d_scene);

    d_scene->OnStartup();

    d_scene->Each<CameraComponent>([&](Entity& entity) {
        d_runtimeCamera = entity;
    });
}

void EditorLayer::OnEvent(Event& event)
{
    if (d_consoleActive) { d_console.OnEvent(event); }
    d_scene->OnEvent(event);
}

void EditorLayer::OnUpdate(double dt)
{
    d_scene->OnUpdate(dt);
    if (d_consoleActive) { d_console.OnUpdate(dt); }
    d_particleManager.OnUpdate(dt);
    
    d_scene->Each<TransformComponent>([&](Entity& entity) {
        auto& transform = entity.Get<TransformComponent>();
        if (transform.position.y < -50) {
            entity.Kill();
        }
    });
    d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
    d_entityRenderer.Draw(d_runtimeCamera, *d_scene);

    if (d_consoleActive) { d_console.Draw(); }
}

}