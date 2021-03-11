#include <Sprocket.h>

#include "Game.h"

using namespace Sprocket;

int main()
{
    //Sprocket::Window window("Game");
    //WorldLayer game(&window);
    //Sprocket::RunOptions options;
    //options.showFramerate = true;
    //return Sprocket::Run(game, window, options);
    log::info("{}", spkt::type_hash<int>);
}
