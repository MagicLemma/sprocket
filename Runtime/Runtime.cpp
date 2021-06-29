#include "Runtime.h"

using namespace spkt;

const auto LIGHT_BLUE  = spkt::from_hex(0x25CCF7);
const auto CLEAR_BLUE  = spkt::from_hex(0x1B9CFC);
const auto GARDEN      = spkt::from_hex(0x55E6C1);
const auto SPACE_DARK  = spkt::from_hex(0x2C3A47);

namespace {

void delete_below_50(spkt::registry& registry, double)
{
    registry.erase_if<Transform3DComponent>([&](apx::entity entity) {
        const auto& t = registry.get<Transform3DComponent>(entity);
        return t.position.y < -50.0f;
    });
}

}

Runtime::Runtime(Window* window) 
    : d_window(window)
    , d_scene(window)
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

    d_scene.Add<PhysicsEngine3D>();
    d_scene.Add<ScriptRunner>();
    d_scene.Add<CameraSystem>();
    d_scene.Add<ParticleSystem>(&d_particleManager);
    d_scene.Add<AnimationSystem>();
    d_scene.add(delete_below_50);
    d_scene.Load("Resources/Anvil.yaml");

    d_runtimeCamera = d_scene.find<Camera3DComponent>();
}

void Runtime::on_event(ev::Event& event)
{
    if (auto data = event.get_if<ev::KeyboardTyped>()) {
        if (data->key == Keyboard::BACK_TICK) {
            d_consoleActive = !d_consoleActive;
            event.consume();
        }
    }

    if (d_consoleActive) {
        d_console.OnEvent(event);
        event.consume();
    }

    d_scene.OnEvent(event);
}

void Runtime::on_update(double dt)
{
    if (d_consoleActive) {
        d_window->SetCursorVisibility(true);
        d_console.OnUpdate(dt);
    } else {
        d_window->SetCursorVisibility(false);
        d_particleManager.OnUpdate(dt);
        d_scene.OnUpdate(dt);
    }
}

void Runtime::on_render()
{
    d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
    d_entityRenderer.Draw(d_runtimeCamera, d_scene);

    if (d_consoleActive) {
        d_console.Draw();
    }
}