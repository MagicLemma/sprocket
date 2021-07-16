#include <Sprocket/Core/Window.h>
#include <Sprocket/Core/GameLoop.h>
#include "Game.h"

using namespace spkt;

int main()
{
    spkt::Window window("Game");
    Game game(&window);
    spkt::RunOptions options;
    options.showFramerate = true;
    return spkt::Run(game, window, options);
}
