#include <Sprocket.h>
#include "Game.h"
#include <any>

class Foo {
public:
    int x = 4;
    Foo() { SPKT_LOG_INFO("Made"); }
    Foo(const Foo& other) { SPKT_LOG_INFO("Copied"); }
    Foo(Foo&& other) { SPKT_LOG_INFO("Moved"); }
    ~Foo() { SPKT_LOG_INFO("Deleted foo"); }
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
    auto e1 = reg.New();
    auto e2 = reg.New();

    NameComponent nc;
    e1.Add(nc);

    TransformComponent tc;
    e1.Add(tc);
    e2.Add(tc);
    e2.Emplace<Foo>();

    auto& f = e2.Get<Foo>();
    f.x = 3;

    auto& f1 = e2.Get<Foo>();
    SPKT_LOG_INFO("{}", f1.x);
}
