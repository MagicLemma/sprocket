#include <Sprocket.h>
#include "Anvil.h"


// See https://gist.github.com/zester/2438462 for better Lua integration

int main()
{
    //Sprocket::Window window("Anvil");
    //Sprocket::Anvil editor(&window);
    //Sprocket::RunOptions options;
    //options.showFramerate = true;
    //return Sprocket::Run(editor, window, options);
    using namespace Sprocket;

    ecs::Index i = 2;
    ecs::Version v = 3;
    ecs::Identifier id = ecs::combine(i, v);

    log::info("{} {} -> {}", i, v, id);

    auto [i2, v2] = ecs::split(id);
    log::info("{} -> {} {}", id, i2, v2);
}
