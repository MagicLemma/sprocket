#pragma once
#include "Window.h"
#include "Stopwatch.h"
#include "Log.h"

#include <exception>

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

    window.SetCallback([&app](Event& event) {
        app.OnEvent(event);
    });

    try {
        while (window.Running()) {
            window.Clear();
            
            double dt = watch.OnUpdate();
            app.OnUpdate(dt);
            app.OnRender();

            if (options.showFramerate) {
                window.SetWindowName(name + " [FPS: " + std::to_string(watch.Framerate()) + "]");
            }

            window.OnUpdate();
        }

    } catch (std::exception& e) {
        SPKT_LOG_FATAL("Unhandled exception: {}", e.what());
        return -1;
    } catch (...) {
        SPKT_LOG_FATAL("Unhandled unknown exception");
        return -2;
    }

    return 0;
}

}