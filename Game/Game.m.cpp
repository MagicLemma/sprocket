#include <Sprocket.h>
#include "WorldLayer.h"

int main()
{
    Sprocket::Window window("Game");

    Sprocket::App app;
    auto worldLayer = app.Add<WorldLayer>(&window);

    Sprocket::RunOptions options;
    options.showFramerate = true;

    return Sprocket::Run(app, window, options);
}