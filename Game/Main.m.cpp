#include <Sprocket.h>
#include "Game.h"

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
    Sprocket::SparseSet<std::string> set;

    set[6] = "a";
    set[2] = "a";
    set[9] = "a";
    set[1] = "a";
    set[0] = "a";
    set[4] = "a";
    set[11] = "a";

    for (auto& [index, value] : set.Fast()) {
        SPKT_LOG_INFO("{} -> {}", index, value);
    }

    SPKT_LOG_INFO("Stable Loop:");

    for (auto& [index, value] : set.Safe()) {
        SPKT_LOG_INFO("{} -> {}", index, value);
        if (index == 9) {
            set.Erase(index);
        }
        value = "6";
    }

    SPKT_LOG_INFO("Fast Loop:");

    for (auto& [index, value] : set.Fast()) {
        SPKT_LOG_INFO("{} -> {}", index, value);
    }


#endif
}
