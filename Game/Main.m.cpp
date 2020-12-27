#include <Sprocket.h>
#include "Game.h"

int main()
{
#define GAME
#ifdef GAME
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#else
    Sprocket::Log::Init();

    SPKT_LOG_INFO("{} {}", sizeof(Sprocket::ECS::Registry*), sizeof(Sprocket::u64));
#endif
}
