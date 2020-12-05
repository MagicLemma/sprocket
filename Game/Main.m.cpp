#include <Sprocket.h>
#include "Game.h"

struct Foo {
    int x = 5;
    Foo(int y) : x(y) {}
    ~Foo() {
        SPKT_LOG_INFO("Deleting just fine!");
    }
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
    reg.Delete(e1);
    SPKT_LOG_INFO("Valid? {}", e1.Valid());

    ECS::Entity e2 = reg.New();
}