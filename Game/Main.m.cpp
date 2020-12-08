#include <Sprocket.h>
#include "Game.h"

struct Foo
{
    int x = 5;
};

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
    SPKT_LOG_INFO("Is e1 valid? {}", e1.Valid());


    reg.OnAdd<NameComponent>([](ECS::Entity e) {
        auto name = e.Get<NameComponent>().name;
        SPKT_LOG_INFO("Added {}", name);
    });

    reg.OnRemove<NameComponent>([](ECS::Entity e) {
        auto name = e.Get<NameComponent>().name;
        SPKT_LOG_INFO("Removed {}", name);
    });

    e1.Add<NameComponent>({"Hello"});
    reg.Delete(e1);
}