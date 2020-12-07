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
    u32 e1 = reg.New();
    u32 e2 = reg.New();
    u32 e3 = reg.New();
    u32 e4 = reg.New();
    u32 e5 = reg.New();
    reg.Add(e1, NameComponent({"E"}));
    reg.Add(e2, NameComponent({"D"}));
    reg.Add(e4, NameComponent({"B"}));
    reg.Add(e5, NameComponent({"A"}));

    reg.Add(e2, Foo());
    reg.Add(e4, Foo());

    for (auto entity : reg.View<NameComponent>()) {
        auto x = reg.Get<NameComponent>(entity);
        SPKT_LOG_INFO("{}", x.name);
    }

    
    for (auto entity : reg.View<Foo>()) {
        auto x = reg.Get<Foo>(entity);
        SPKT_LOG_INFO("{}", x.x);
    }

}