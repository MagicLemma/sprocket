#pragma once
#include <sprocket/Core/events.h>
#include <sprocket/Core/window.h>
#include <sprocket/Core/log.h>
#include <sprocket/Core/timer.h>

#include <concepts>
#include <exception>
#include <format>
#include <type_traits>
#include <utility>

namespace spkt {

struct run_options
{
    bool show_frame_rate = false;
};

template <typename T>
concept runnable = requires(T t, event& event, double dt)
{
    { t.on_event(event) } -> std::same_as<void>;
    { t.on_update(dt) } -> std::same_as<void>;
    { t.on_render() } -> std::same_as<void>;
};

template <runnable App>
int run(App& app, window& window, const run_options& options = {})
{
    std::string name = window.name();
    timer watch;

    window.set_event_handler([&app](event& event) {
        app.on_event(event);
    });

    while (window.is_running()) {
        window.begin_frame();

        double dt = watch.on_update();
        app.on_update(dt);
        app.on_render();

        if (options.show_frame_rate) {
            window.set_name(std::format("{} [FPS: {}]", name, watch.frame_rate()));
        }

        window.end_frame();
    }

    return 0;
}

// If the application type only needs a window* to be constructed, the entire run loop
// can be further encapsulated for the user.
template <runnable App> requires std::constructible_from<App, spkt::window*>
int run_app(const std::string& name)
{
    spkt::window window{name};
    App app{&window};
    return spkt::run(app, window);
}

template <runnable App> requires std::constructible_from<App, spkt::window*>
int run_app_framerate(const std::string& name)
{
    spkt::window window{name};
    App app{&window};
    spkt::run_options options;
    options.show_frame_rate = true;
    return spkt::run(app, window, options);
}

}