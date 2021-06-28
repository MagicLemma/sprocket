#include <Sprocket.h>
#include "Game.h"

using namespace spkt;

int main()
{
    spkt::Window window("Game");
    WorldLayer game(&window);
    spkt::RunOptions options;
    options.showFramerate = true;
    return spkt::Run(game, window, options);
}
