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
    u64 e1 = reg.New();
    u64 e2 = reg.New();
    u64 e3 = reg.New();
    u64 e6 = reg.New();
    u64 e4 = reg.New();
    u64 e5 = reg.New();
    reg.Add(e1, NameComponent({"E"}));
    reg.Add(e2, NameComponent({"D"}));
    reg.Add(e3, NameComponent({"C"}));
    reg.Add(e4, NameComponent({"B"}));
    reg.Add(e5, NameComponent({"A"}));
    
    for (auto entity : reg) {
        SPKT_LOG_INFO("{}", entity);
    }
}