#include <Sprocket.h>
#include "Game.h"

struct A {};
struct B {};
struct C {};

template <typename T>
bool bar()
{
    return true;
}

template <typename... Types>
void foo ()
{
    if ((bar<Types>() && ...)) {
        SPKT_LOG_INFO("True!");
    }
    else {
        SPKT_LOG_INFO("False!");
    }
}

int main()
{
#ifdef GAME
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#else
    Sprocket::Log::Init();
    foo<>();
#endif
}
