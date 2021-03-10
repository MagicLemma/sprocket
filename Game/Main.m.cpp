#include <Sprocket.h>
#include <unordered_map>

#include "Game.h"

using namespace Sprocket;

int main()
{
    //Sprocket::Window window("Game");
    //WorldLayer game(&window);
    //Sprocket::RunOptions options;
    //options.showFramerate = true;
    //return Sprocket::Run(game, window, options);
    log::info("{}", type_hash<int>);
    log::info("{}", type_hash<ecs::Entity>);
    log::info("{}", type_hash<float>);
    log::info("{}", type_hash<int>);

    static_assert(type_hash<int> > 0);
}
