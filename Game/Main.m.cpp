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
#if 0
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#endif
    Sprocket::Log::Init();
    using namespace Sprocket;
    std::random_device rd;
    SPKT_LOG_INFO("{}", rd());
    SPKT_LOG_INFO("{}", rd());
    SPKT_LOG_INFO("{}", rd());
}
