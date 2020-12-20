#include <Sprocket.h>
#include "Game.h"

struct A {
    static const int x = 5;
};
struct B {
    static const int x = 2;
};
struct C {
    static const int x = 6;
};

template <typename Comp, typename... Rest>
constexpr std::pair<std::type_index, int> get()
{
    if constexpr (sizeof...(Rest) == 0) {
        return {typeid(Comp), Comp::x};
    } else {
        return std::min(get<Comp>(), get<Rest...>(), [](const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second;
        });
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
    auto [index, value] = get<A, B, C>();
    SPKT_LOG_INFO("{}", index.name());
#endif
}
