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

    lua::Script script;
    double value = script.call_function<double>("Sin", 50);
    log::info("Got value {}", value);
}
