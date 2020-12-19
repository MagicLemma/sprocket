#include <Sprocket.h>
#include "Game.h"

int main()
{
#ifdef GAME
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#else
    Sprocket::Log::Init();
    Sprocket::ECS::Registry reg;

    auto e1 = reg.New();

    auto e2 = reg.New();
    SPKT_LOG_INFO("SLOT {} VERSION {}", e2.Slot(), e2.Version());

    for (auto e : reg.All()) {
        SPKT_LOG_INFO("{}: {} {}", e.Id(), e.Slot(), e.Version());
    }

#endif
}
