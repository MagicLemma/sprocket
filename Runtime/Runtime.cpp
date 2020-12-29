#include "Runtime.h"

using namespace Sprocket;

const auto LIGHT_BLUE  = Sprocket::FromHex(0x25CCF7);
const auto CLEAR_BLUE  = Sprocket::FromHex(0x1B9CFC);
const auto GARDEN      = Sprocket::FromHex(0x55E6C1);
const auto SPACE_DARK  = Sprocket::FromHex(0x2C3A47);

Runtime::Runtime(Window* window) 
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
    , d_console(d_window)
{
    d_window->SetCursorVisibility(false);
    d_entityRenderer.EnableParticles(&d_particleManager);

    d_scene.Add<PhysicsEngine>();
    d_scene.Add<ScriptRunner>(d_window);
    d_scene.Add<CameraSystem>(d_window->AspectRatio());
    d_scene.Add<ParticleSystem>(&d_particleManager);
    d_scene.Add<AnimationSystem>();
    d_scene.Load("Resources/Anvil.yaml");

    d_runtimeCamera = d_scene.Entities().Find<CameraComponent>();
}

void Runtime::OnEvent(Event& event)
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

    d_scene.OnEvent(event);
}

void Runtime::OnUpdate(double dt)
{
    d_window->SetCursorVisibility(d_consoleActive);

    if (d_consoleActive) {
        d_console.OnUpdate(dt);
    } else {
        d_scene.OnUpdate(dt);
        d_particleManager.OnUpdate(dt);
    }
    
    std::vector<ECS::Entity> toDelete;
    for (auto entity : d_scene.Entities().View<TransformComponent>()) {
        auto& transform = entity.Get<TransformComponent>();
        if (transform.position.y < -50) {
            toDelete.push_back(entity);
        }
    }
    for (auto entity : toDelete) {
        entity.Delete();
    }
}

void Runtime::OnRender()
{
    d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
    d_entityRenderer.Draw(d_runtimeCamera, d_scene);

    if (d_consoleActive) {
        d_console.Draw();
    }
}