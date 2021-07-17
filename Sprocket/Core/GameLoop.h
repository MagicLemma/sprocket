#pragma once
#include <Sprocket/Core/Events.h>
#include <Sprocket/Core/Window.h>
#include <Sprocket/Utility/Log.h>
#include <Sprocket/Utility/Stopwatch.h>

#include <exception>
#include <format>
#include <type_traits>
#include <utility>

namespace spkt {

struct RunOptions
{
    bool showFramerate = false;
};

template <typename T>
concept runnable = requires(T t, event& event, double dt)
{
    { t.on_event(event) } -> std::same_as<void>;
    { t.on_update(dt) } -> std::same_as<void>;
    { t.on_render() } -> std::same_as<void>;
};

template <runnable App>
int Run(App& app, Window& window, const RunOptions& options = {})
{
    std::string name = window.GetWindowName();
    Stopwatch watch;

    window.SetCallback([&app](event& event) {
        app.on_event(event);
    });

    while (window.Running()) {
        window.Clear();

        double dt = watch.on_update();
        app.on_update(dt);
        app.on_render();

        if (options.showFramerate) {
            window.SetWindowName(std::format("{} [FPS: {}]", name, watch.Framerate()));
        }

        window.on_update();
    }

    return 0;
}

}