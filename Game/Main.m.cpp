#include <Sprocket.h>
#include <mutex>

#include "Game.h"

using namespace Sprocket;

int main()
{
    log::fatal("{}", spkt::type_hash<float>);
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
}
