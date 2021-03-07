#include <Sprocket.h>

#include "Game.h"

#include <cpp-itertools.hpp>
#include <memory>
#include <functional>

using namespace Sprocket;

void foo(const std::string& t)
{
    log::info("string");
}

template <typename T, typename = std::enable_if_t<!std::is_same_v<T, const char>, T>>
void foo(T* x)
{
    log::info("T*");
}

int main()
{
    //Sprocket::Window window("Game");
    //WorldLayer game(&window);
    //Sprocket::RunOptions options;
    //options.showFramerate = true;
    //return Sprocket::Run(game, window, options);

    foo("hello");
}
