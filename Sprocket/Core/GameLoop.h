#pragma once
#include "Window.h"
#include "Stopwatch.h"
#include "Log.h"

#include <exception>
#include <format>

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
        window.OnUpdate();
        app.OnUpdate(dt);
        app.OnRender();

        if (options.showFramerate) {
            window.SetWindowName(std::format("{} [FPS: {}]", name, watch.Framerate()));
        }

    }

    return 0;
}

}