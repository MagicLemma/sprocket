#include <Sprocket.h>
#include "Game.h"

struct Foo {};
struct Bar {};
struct Baz {};

int main()
{
#define GAME
#ifdef GAME
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#else
    using namespace Sprocket;
    Log::Init();

    ECS::Registry reg;
    auto e1 = reg.New();
    auto e2 = reg.New();

    e1.Add<Foo>();
    
    e2.Add<Bar>();

    e1.Add<Baz>();
    e2.Add<Baz>();

    SPKT_LOG_INFO("{}", reg.Count());
    SPKT_LOG_INFO("{}", reg.Count<Foo>());
    SPKT_LOG_INFO("{}", reg.Count<Baz>());
    SPKT_LOG_INFO("{}", reg.Count<Baz>(ECS::Exclude<Bar, Baz>()));
#endif
}
