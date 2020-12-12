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
    auto e2 = reg.New();

    NameComponent nc;
    e1.Add(nc);

    TransformComponent tc;
    e1.Add(tc);
    e2.Add(tc);

    for (auto e : reg.View<NameComponent, TransformComponent>()) {
        SPKT_LOG_INFO("Loop!");
    }
}
