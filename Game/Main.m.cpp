#include <Sprocket.h>

#include "Game.h"

#include <cpp-itertools.hpp>

double foo(int x, double y) {
    return y + x;
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
    using namespace Sprocket;
    Log::Init();

    std::vector<int> data{1, 4, 6, 4, 1};

    std::vector<std::tuple<int, double>> args{
        {1, 2.0},
        {3, 4.5}
    };

    for (const auto& x : itertools::starmap(foo, args)) {
        SPKT_LOG_INFO("{}", x);
    }


#endif
}
