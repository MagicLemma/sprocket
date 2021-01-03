#include <Sprocket.h>
#include "Game.h"

struct Foo {};
struct Bar {};
struct Baz {};

int main()
{
#ifdef GAME
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#else
    using namespace Sprocket;
    Log::Init();

    std::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2{6, 7, 8, 9};

    for (auto& [l, r] : itertools::zip(v1, v2)) {
        SPKT_LOG_INFO("{}, {}", l, r);
    }


#endif
}
