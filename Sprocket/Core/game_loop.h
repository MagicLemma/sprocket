#pragma once
#include <Sprocket/Core/events.h>
#include <Sprocket/Core/Window.h>
#include <Sprocket/Core/log.h>
#include <Sprocket/Core/timer.h>

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
    std::string name = window.GetWindowName();
    timer watch;

    window.SetCallback([&app](event& event) {
        app.on_event(event);
    });

    while (window.Running()) {
        window.begin_frame();

        double dt = watch.on_update();
        app.on_update(dt);
        app.on_render();

        if (options.show_frame_rate) {
            window.SetWindowName(std::format("{} [FPS: {}]", name, watch.frame_rate()));
        }

        window.end_frame();
    }

    return 0;
}

}