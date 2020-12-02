#include "EditorLayer.h"

namespace Sprocket {

const auto LIGHT_BLUE  = Sprocket::FromHex(0x25CCF7);
const auto CLEAR_BLUE  = Sprocket::FromHex(0x1B9CFC);
const auto GARDEN      = Sprocket::FromHex(0x55E6C1);
const auto SPACE_DARK  = Sprocket::FromHex(0x2C3A47);

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
    d_core.window->SetCursorVisibility(false);
    d_entityRenderer.EnableParticles(&d_particleManager);

    d_scene = std::make_shared<Scene>();    
    d_scene->AddSystem(std::make_shared<PhysicsEngine>(glm::vec3{0.0, -9.81, 0.0}));
    d_scene->AddSystem(std::make_shared<CameraSystem>(d_core.window->AspectRatio()));
    d_scene->AddSystem(std::make_shared<ScriptRunner>(d_core.window));
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
    if (auto e = event.As<KeyboardKeyTypedEvent>()) {
        if (e->Key() == Keyboard::BACK_TICK) {
            d_consoleActive = !d_consoleActive;
            e->Consume();
        }
    }

    if (d_consoleActive) {
        d_console.OnEvent(event);
        event.Consume();
    }
    d_scene->OnEvent(event);
}

void EditorLayer::OnUpdate(double dt)
{
    d_core.window->SetCursorVisibility(d_consoleActive);

    if (d_consoleActive) {
        d_console.OnUpdate(dt);
    } else {
        d_scene->OnUpdate(dt);
        d_particleManager.OnUpdate(dt);
    }
    
    d_scene->Each<TransformComponent>([&](Entity& entity) {
        auto& transform = entity.Get<TransformComponent>();
        if (transform.position.y < -50) {
            entity.Kill();
        }
    });
    
    d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
    d_entityRenderer.Draw(d_runtimeCamera, *d_scene);

    if (d_consoleActive) {
        d_console.Draw();
    }
}

}