#include "Runtime.h"

#include <Sprocket/Core/Events.h>
#include <Sprocket/Core/Window.h>
#include <Sprocket/Scene/loader.h>
#include <Sprocket/Scene/Systems/basic_systems.h>
#include <Sprocket/Scene/Systems/particle_system.h>
#include <Sprocket/Scene/Systems/physics_system.h>
#include <Sprocket/Scripting/LuaScript.h>
#include <Sprocket/Utility/Colour.h>
#include <Sprocket/Utility/KeyboardCodes.h>

const auto LIGHT_BLUE  = spkt::from_hex(0x25CCF7);
const auto CLEAR_BLUE  = spkt::from_hex(0x1B9CFC);
const auto GARDEN      = spkt::from_hex(0x55E6C1);
const auto SPACE_DARK  = spkt::from_hex(0x2C3A47);

Runtime::Runtime(spkt::Window* window) 
    : d_window(window)
    , d_scene({
        .registry = {},
        .systems = {
            spkt::physics_system,
            spkt::particle_system,
            spkt::script_system,
            spkt::camera_system,
            spkt::animation_system,
            spkt::delete_below_50_system,
            spkt::clear_events_system
        }
    })
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

    spkt::add_singleton(d_scene.registry);
    spkt::load_registry_from_file("Resources/Anvil.yaml", d_scene.registry);

    d_runtimeCamera = d_scene.registry.find<spkt::Camera3DComponent>();

    d_console.register_command("clear", [](Console& console, auto args) {
        console.clear_history();
    });
    
    d_console.register_command("exit", [&](Console& console, auto args) {
        d_window->Close();
    });

    d_console.register_command("echo", [](Console& console, auto args) {
        if (args.size() < 2) { return; }
        std::string echo = args[1];
        for (auto arg : args | std::views::drop(2)) echo += " " + arg;
        console.log(" > {}", echo);
    });

    d_console.register_command("run", [](Console& console, auto args) {
        if (args.size() != 2) {
            console.error("Invalid args for {}", args[0]);
            return;
        }
        const auto script_file = std::format("Resources/Scripts/{}", args[1]);
        if (std::filesystem::exists(script_file)) {
            spkt::lua::Script script(script_file);
        } else {
            console.error(" > Could not find script '{}'", args[1]);
        }
    });
}

void Runtime::on_event(spkt::event& event)
{
    if (auto data = event.get_if<spkt::keyboard_typed_event>()) {
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
        d_scene.on_update(dt);
    }
}

void Runtime::on_render()
{
    d_skyboxRenderer.Draw(d_skybox, d_scene.registry, d_runtimeCamera);
    d_entityRenderer.Draw(d_scene.registry, d_runtimeCamera);

    if (d_consoleActive) {
        d_console.draw();
    }
}