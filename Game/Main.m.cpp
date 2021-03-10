#include <Sprocket.h>

#include "Game.h"

using namespace Sprocket;

template <typename T>
constexpr const char* foo()
{
    return __FUNCSIG__;
}

constexpr std::size_t sdbm_hash2(const char* text, std::size_t hash = 0)
{
    if (text[0] == '\0') { return hash; } // We are done
    std::size_t char_val = static_cast<std::size_t>(text[0]);
    return sdbm_hash2(&text[1], char_val + (hash << 6) + (hash << 16) - hash);
};

template <typename T>
constexpr std::size_t type_hash2 = sdbm_hash2(foo<T>());

template <std::size_t hash, typename T, typename = std::enable_if_t<type_hash<T> == hash, T>>
T bar()
{
    return T{};
}

int main()
{
    //Sprocket::Window window("Game");
    //WorldLayer game(&window);
    //Sprocket::RunOptions options;
    //options.showFramerate = true;
    //return Sprocket::Run(game, window, options);

    log::info("{}", foo<std::size_t>());
    log::info("{}", foo<unsigned long long>());
}
