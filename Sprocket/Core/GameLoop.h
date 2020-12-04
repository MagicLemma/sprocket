#pragma once
#include "Window.h"
#include "Stopwatch.h"

namespace Sprocket {

template <typename App>
class GameLoop
{
    App*    d_app;
    Window* d_window;

    Stopwatch d_stopwatch;

public:
    GameLoop(App* app, Window* window) : d_app(app), d_window(window) {}

    void Run();
};

template <typename App>
void GameLoop<App>::Run()
{
    while (d_window->Running()) {
        double dt = d_stopwatch.DeltaTime();

        d_window->Clear();
        
        d_stopwatch.OnUpdate();
        d_app->OnUpdate(dt);
        d_app->OnRender();

        d_window->OnUpdate();
    }
}

}