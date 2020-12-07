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
    ECS::Entity e2 = reg.New();
    ECS::Entity e3 = reg.New();
    ECS::Entity e4 = reg.New();
    ECS::Entity e5 = reg.New();

    reg.OnAdd<NameComponent>([](ECS::Entity e) {
        auto name = e.Get<NameComponent>().name;
        SPKT_LOG_INFO("Added {}", name);
    });

    reg.OnRemove<NameComponent>([](ECS::Entity e) {
        auto name = e.Get<NameComponent>().name;
        SPKT_LOG_INFO("Removed {}", name);
    });

    e1.Add(NameComponent({"E"}));
    e2.Add(NameComponent({"D"}));
    e4.Add(NameComponent({"B"}));
    e5.Add(NameComponent({"A"}));

    e2.Remove<NameComponent>();

    e2.Add(Foo());
    e4.Add(Foo());

    for (auto entity : reg.View<NameComponent>()) {
        auto x = entity.Get<NameComponent>();
        SPKT_LOG_INFO("{}", x.name);
    }

    bool isNull = e2 == ECS::Null;
    SPKT_LOG_INFO("Comparison to null {}", isNull);
    
    for (auto entity : reg.View<Foo>()) {
        auto x = entity.Get<Foo>();
        SPKT_LOG_INFO("{}", x.x);
    }

}