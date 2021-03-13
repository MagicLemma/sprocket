#include <Sprocket.h>

#include "Game.h"

using namespace Sprocket;

struct Foo
{
    int x;
    double y;
    float z;
};

int main()
{
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
}
