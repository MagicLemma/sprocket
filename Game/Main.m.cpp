#include <Sprocket.h>

#include "Game.h"

#include <cpp-itertools.hpp>
#include <memory>
#include <functional>

using namespace Sprocket;

int main()
{
    //Sprocket::Window window("Game");
    //WorldLayer game(&window);
    //Sprocket::RunOptions options;
    //options.showFramerate = true;
    //return Sprocket::Run(game, window, options);

    lua::Script script("Resources/Temp.lua");
    int value = script.call_function<int>("foo", 50);
    log::info("Got value {}", value);
}
