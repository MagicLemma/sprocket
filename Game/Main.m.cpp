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
    reg.Delete(e1);
    auto e2 = reg.New();

    SPKT_LOG_INFO("Slots    {} {}", reg.GetSlot(e1), reg.GetSlot(e2));
    SPKT_LOG_INFO("Versions {} {}", reg.GetVersion(e1), reg.GetVersion(e2));
    SPKT_LOG_INFO("Valid?   {} {}", reg.Valid(e1), reg.Valid(e2));
}
