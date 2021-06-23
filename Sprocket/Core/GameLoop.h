#pragma once
#include "Window.h"
#include "Stopwatch.h"
#include "Log.h"
#include "Events.h"

#include <exception>
#include <format>
#include <utility>
#include <GLFW/glfw3.h>

namespace Sprocket {

struct RunOptions
{
    bool showFramerate = false;
};

template <typename T>
concept legacy_runnable = requires(T t, ev::Event& event, double dt)
{
    { t.OnEvent(event) } -> std::same_as<void>;
    { t.OnUpdate(dt) } -> std::same_as<void>;
    { t.OnRender() } -> std::same_as<void>;
};

template <typename T>
concept runnable = requires(T t, ev::Event& event, double dt)
{
    { t.on_event(event) } -> std::same_as<void>;
    { t.on_update(dt) } -> std::same_as<void>;
    { t.on_render() } -> std::same_as<void>;
};

template <legacy_runnable App>
int Run(App& app, Window& window, const RunOptions& options = {})
{
    std::string name = window.GetWindowName();
    Stopwatch watch;

    window.SetCallback([&app](ev::Event& event) {
        app.OnEvent(event);
    });

    while (window.Running()) {
        window.Clear();
        
        double dt = watch.OnUpdate();
        app.OnUpdate(dt);
        app.OnRender();

        if (options.showFramerate) {
            window.SetWindowName(std::format("{} [FPS: {}]", name, watch.Framerate()));
        }

        window.OnUpdate();
    }

    return 0;
}

template <runnable App>
int Run(App& app, Window& window, const RunOptions& options = {})
{
    std::string name = window.GetWindowName();
    Stopwatch watch;

    window.SetCallback([&app](ev::Event& event) {
        app.on_event(event);
    });

    while (window.Running()) {
        window.Clear();

        double dt = watch.OnUpdate();
        app.on_update(dt);
        app.on_render();

        if (options.showFramerate) {
            window.SetWindowName(std::format("{} [FPS: {}]", name, watch.Framerate()));
        }

        window.OnUpdate();
    }

    return 0;
}

}