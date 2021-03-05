#include <Sprocket.h>

#include "Game.h"

#include <cpp-itertools.hpp>

using namespace Sprocket;

void foo(int val)
{
    log::info("foo<int>");
}

void foo(double val)
{
    log::info("foo<double>");
}

void foo(float val)
{
    log::info("foo<float>");
}

void foo(bool val)
{
    log::info("foo<bool>");
}

void foo(const char* val)
{
    log::info("foo<const char*>");
}

void foo(const std::string& val)
{
    log::info("foo<std::string>");
}

template <typename T>
void foo(T* val)
{
    log::info("foo<T*>");
}

void foo(void* val)
{
    log::info("foo<void*>");
}

template <typename T>
void foo(const T& val)
{
    static_assert(std::is_copy_assignable_v<T>);
    static_assert(std::is_trivially_destructible_v<T>);
    log::info("foo<T> (copy assignable and trivially destrucible)");
}

struct EntityFake
{
    int x;
    float y;
    double z;
    EntityFake() : x(1), y(2.0f), z(3.0) {}
};

int main()
{
#if 0
    Sprocket::Window window("Game");
    WorldLayer game(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(game, window, options);
#endif
    log::init();

    int x = 5;

    foo(x);
    foo(&x);
}
