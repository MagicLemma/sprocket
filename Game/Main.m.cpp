#include <Sprocket.h>
#include "Game.h"

int main()
{
    Sprocket::Window window("Game");
    WorldLayer game(&window);

    Sprocket::RunOptions options;
    options.showFramerate = true;

    return Sprocket::Run(game, window, options);
}