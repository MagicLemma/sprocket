#include <Sprocket.h>

#include "Game.h"

#include <cpp-itertools.hpp>

struct Foo
{
    int* x;
    uint8_t y;
    uint32_t z[4];
};

int main()
{
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
}
