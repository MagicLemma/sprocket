#include "Runtime.h"

const auto LIGHT_BLUE  = spkt::from_hex(0x25CCF7);
const auto CLEAR_BLUE  = spkt::from_hex(0x1B9CFC);
const auto GARDEN      = spkt::from_hex(0x55E6C1);
const auto SPACE_DARK  = spkt::from_hex(0x2C3A47);

Runtime::Runtime(spkt::Window* window) 
    : d_window(window)
    , d_scene()
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

    spkt::add_singleton(d_scene.Entities());
    spkt::particle_system_init(d_scene.Entities(), &d_particleManager);
    spkt::load_registry_from_file("Resources/Anvil.yaml", &d_scene.Entities());
    
    d_scene.add(spkt::physics_system);
    d_scene.add(spkt::particle_system);
    d_scene.add(spkt::script_system);
    d_scene.add(spkt::camera_system);
    d_scene.add(spkt::animation_system);
    d_scene.add(spkt::delete_below_50_system);
    d_scene.add(spkt::clear_events_system);

    d_runtimeCamera = d_scene.Entities().find<spkt::Camera3DComponent>();
}

void Runtime::on_event(spkt::ev::Event& event)
{
    if (auto data = event.get_if<spkt::ev::KeyboardTyped>()) {
        if (data->key == spkt::Keyboard::BACK_TICK) {
            d_consoleActive = !d_consoleActive;
            event.consume();
        }
    }

    if (d_consoleActive) {
        d_console.on_event(event);
        event.consume();
    }

    d_scene.on_event(event);
}

void Runtime::on_update(double dt)
{
    if (d_consoleActive) {
        d_window->SetCursorVisibility(true);
        d_console.on_update(dt);
    } else {
        d_window->SetCursorVisibility(false);
        d_particleManager.on_update(d_scene.Entities(), dt);
        d_scene.on_update(dt);
    }
}

void Runtime::on_render()
{
    auto& registry = d_scene.Entities();
    d_skyboxRenderer.Draw(d_skybox, registry, d_runtimeCamera);
    d_entityRenderer.Draw(registry, d_runtimeCamera);

    if (d_consoleActive) {
        d_console.Draw();
    }
}