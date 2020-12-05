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

    //Registry reg;
//
    //u32 entity = reg.New();
    //SPKT_LOG_INFO("Entity: {}", entity);

    TransformComponent tc;
    tc.position = {1, 2, 3};
    tc.orientation = {1, 2, 3, 4};
    tc.scale = {9, 8, 7};


    {
        TypeErasedComponent tec(Foo(5));
        auto x = tec.Get<Foo>().x;
        SPKT_LOG_INFO("Value = {}", x);
    }

    SPKT_LOG_INFO("Outside");
}