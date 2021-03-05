#include <Sprocket.h>

#include "Game.h"

#include <cpp-itertools.hpp>
#include <memory>
#include <functional>

using namespace Sprocket;

int main()
{
#if 0
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#endif
    log::init();

    log::info("{}", sizeof(std::unique_ptr<int>));
    log::info("{}", sizeof(std::unique_ptr<int, void(*)(int*)>));
}
