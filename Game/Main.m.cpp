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
    SPKT_LOG_INFO("Size of int = {}", sizeof(int));
    SPKT_LOG_INFO("Size of int& = {}", sizeof(int&));
    SPKT_LOG_INFO("Size of guid::GUID = {}", sizeof(guid::GUID));
    SPKT_LOG_INFO("Size of ecs::Entity = {}", sizeof(ecs::Entity));
    SPKT_LOG_INFO("Size of Foo = {}", sizeof(Foo));
    SPKT_LOG_INFO("Size of uint64 = {}", sizeof(uint64_t));
    SPKT_LOG_INFO("Size of uint64 = {}", sizeof(std::size_t));
}
