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
    ECS::Entity e1 = reg.New();
    e1.Add<NameComponent>({"A"});

    ECS::Entity e2 = reg.New();
    e2.Add<NameComponent>({"D"});

    ECS::Entity e3 = reg.New();
    e3.Add<NameComponent>({"B"});

    ECS::Entity e4 = reg.New();
    e4.Add<NameComponent>({"C"});
}
