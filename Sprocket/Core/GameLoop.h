#pragma once
#include "Window.h"
#include "Stopwatch.h"
#include "Log.h"

#include <exception>
#include <fmt/core.h>

namespace Sprocket {

struct RunOptions
{
    bool showFramerate = false;
};

template <typename App>
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
            window.SetWindowName(fmt::format("{} [FPS: {}]", name, watch.Framerate()));
        }

        window.OnUpdate();
    }

    return 0;
}

}