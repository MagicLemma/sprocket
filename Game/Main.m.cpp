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

    set.Insert(4, "Whoops");
    set.Insert(4, "200");

    set[2] = "Hello";
    set[10] = "world";
    set[5] = "Matt";

    SPKT_LOG_INFO("Size? {}", set.Size());
    SPKT_LOG_INFO("Has 2? {}", set.Has(2));
    SPKT_LOG_INFO("Get 2: {}", set[2]);

    for (const auto& [key, val] : set) {
        SPKT_LOG_INFO("{} -> {}", key, val);
    }

    SPKT_LOG_INFO("Clearing");

    set.Clear();
    SPKT_LOG_INFO("{}", set.Size());

    for (const auto& [key, val] : set) {
        SPKT_LOG_INFO("{} -> {}", key, val);
    }

    set[5] = "Matt";

    for (const auto& [key, val] : set) {
        SPKT_LOG_INFO("{} -> {}", key, val);
    }

#endif
}
