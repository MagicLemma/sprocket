#include "Runtime.h"

#include <Anvil/particle_system.h>
#include <Anvil/physics_system.h>
#include <Anvil/rendering.h>
#include <Anvil/scene_utils.h>
#include <Anvil/systems.h>
#include <Anvil/ecs/loader.h>

#include <Sprocket/Core/events.h>
#include <Sprocket/Core/Window.h>
#include <Sprocket/Scripting/lua_script.h>
#include <Sprocket/UI/console.h>
#include <Sprocket/Utility/Colour.h>
#include <Sprocket/Graphics/camera.h>
#include <Sprocket/Core/input_codes.h>

#include <Sprocket/Graphics/Rendering/shape_renderer.h>

const auto LIGHT_BLUE  = spkt::from_hex(0x25CCF7);
const auto CLEAR_BLUE  = spkt::from_hex(0x1B9CFC);
const auto GARDEN      = spkt::from_hex(0x55E6C1);
const auto SPACE_DARK  = spkt::from_hex(0x2C3A47);

Runtime::Runtime(spkt::window* window) 
    : d_window(window)
    , d_scene({
        .registry = {},
        .systems = {
            physics_system,
            anvil::particle_system,
            anvil::script_system,
            anvil::animation_system,
            anvil::delete_below_50_system,
            anvil::clear_events_system,
            anvil::input_system_end
        },
        .event_handlers = {
            anvil::input_system_on_event
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
    , d_ui(d_window)
    , d_console()
{
    d_window->set_cursor_visibility(false);

    anvil::input_system_init(d_scene.registry, d_window);
    anvil::load_registry_from_file("Resources/Anvil.yaml", d_scene.registry);

    d_runtimeCamera = d_scene.registry.find<anvil::Camera3DComponent>();

    spkt::SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.backgroundColour.w = 0.8f;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_ui.SetTheme(theme);

    d_console.register_command("clear", [](spkt::console& console, auto args) {
        console.clear_history();
    });
    
    d_console.register_command("exit", [&](spkt::console& console, auto args) {
        d_window->close();
    });

    d_console.register_command("echo", [](spkt::console& console, auto args) {
        if (args.size() < 2) { return; }
        std::string echo = args[1];
        for (auto arg : args | std::views::drop(2)) echo += " " + arg;
        console.log(" > {}", echo);
    });

    d_console.register_command("run", [](spkt::console& console, auto args) {
        if (args.size() != 2) {
            console.error("Invalid args for {}", args[0]);
            return;
        }
        const auto script_file = std::format("Resources/Scripts/{}", args[1]);
        if (std::filesystem::exists(script_file)) {
            spkt::lua::script script(script_file);
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
        auto data = event.get_if<spkt::keyboard_pressed_event>();
        if (data && data->key == spkt::Keyboard::ENTER) {
            d_console.submit(d_command_line);
            d_command_line.clear();
            event.consume();
        } else {
            d_ui.on_event(event);
        }
        event.consume();
    }

    d_scene.on_event(event);
}

void Runtime::on_update(double dt)
{
    d_ui.on_update(dt);
    d_window->set_cursor_visibility(d_consoleActive);
    if (!d_consoleActive) {
        d_scene.on_update(dt);
    }
}

void Runtime::on_render()
{
    auto [proj, view] = anvil::get_proj_view_matrices(d_scene.registry, d_runtimeCamera);
    d_skyboxRenderer.Draw(d_skybox, proj, view);
    anvil::draw_scene(d_entityRenderer, d_scene.registry, proj, view);

    if (d_consoleActive) {
        d_ui.StartFrame();
        draw_console(d_console, d_command_line, d_ui, d_window->width(), d_window->height());
        d_ui.EndFrame();
    }

    static spkt::shape_renderer shapes;
    shapes.begin_frame((float)d_window->width(), (float)d_window->height());
    shapes.draw_line({100.0, 100.0}, {200.0, 200.0}, {1.0, 0.0, 0.0, 1.0}, 1.0f);
    shapes.end_frame();
}