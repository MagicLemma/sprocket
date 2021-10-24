#include "Runtime.h"
#include <anvil/particle_system.h>
#include <anvil/physics_system.h>
#include <anvil/rendering.h>
#include <anvil/scene_utils.h>
#include <anvil/systems.h>
#include <anvil/ecs/loader.h>

#include <sprocket/Core/events.h>
#include <sprocket/Core/Window.h>
#include <sprocket/Scripting/lua_script.h>
#include <sprocket/UI/console.h>
#include <sprocket/Utility/colour.h>
#include <sprocket/Graphics/camera.h>
#include <sprocket/Core/input_codes.h>

const auto LIGHT_BLUE  = spkt::from_hex(0x25CCF7);
const auto CLEAR_BLUE  = spkt::from_hex(0x1B9CFC);
const auto GARDEN      = spkt::from_hex(0x55E6C1);
const auto SPACE_DARK  = spkt::from_hex(0x2C3A47);

namespace anvil {

runtime::runtime(spkt::window* window) 
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
    , d_asset_manager()
    , d_scene_renderer(&d_asset_manager)
    , d_skybox_renderer()
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

    d_runtime_camera = d_scene.registry.find<anvil::Camera3DComponent>();

    spkt::simple_ui_theme theme;
    theme.background_colour = SPACE_DARK;
    theme.background_colour.w = 0.8f;
    theme.base_colour = CLEAR_BLUE;
    theme.hovered_colour = LIGHT_BLUE;
    theme.clicked_colour = GARDEN;
    d_ui.set_theme(theme);

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

void runtime::on_event(spkt::event& event)
{
    if (auto data = event.get_if<spkt::keyboard_typed_event>()) {
        if (data->key == spkt::Keyboard::BACK_TICK) {
            d_console_active = !d_console_active;
            event.consume();
        }
    }

    if (d_console_active) {
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

void runtime::on_update(double dt)
{
    d_ui.on_update(dt);
    d_window->set_cursor_visibility(d_console_active);
    if (!d_console_active) {
        d_scene.on_update(dt);
    }
}

void runtime::on_render()
{
    auto [proj, view] = anvil::get_proj_view_matrices(d_scene.registry, d_runtime_camera);
    d_skybox_renderer.draw(d_skybox, proj, view);
    anvil::draw_scene(d_scene_renderer, d_scene.registry, proj, view);

    if (d_console_active) {
        d_ui.start_frame();
        draw_console(d_console, d_command_line, d_ui, d_window->width(), d_window->height());
        d_ui.end_frame();
    }
}

}