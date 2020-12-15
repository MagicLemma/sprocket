#include <Sprocket.h>
#include "Game.h"

int main()
{
    //Sprocket::Window window("Game");
    //WorldLayer game(&window);
    //Sprocket::RunOptions options;
    //options.showFramerate = true;
    //return Sprocket::Run(game, window, options);

    using namespace Sprocket;
    Log::Init();

    ECS::Registry reg;

    auto e1 = reg.New();
    reg.Emplace<NameComponent>(e1);
    auto e2 = reg.New();
    auto e3 = reg.New();

    for (auto e : reg) {
        SPKT_LOG_INFO("LOOP");
        if (e.Has<NameComponent>()) {
            e.Delete();
        }
    }

    for (auto e : reg) {
        SPKT_LOG_INFO("LOOP 2");
    }
}
