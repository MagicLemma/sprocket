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
    glm::vec3 x{1.1234, 2.4567, 3.00343};
    auto pr = Sprocket::Printer::PrintVec3(x, 1);
    SPKT_LOG_INFO("{}", pr);
}
