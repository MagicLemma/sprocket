#include <Anvil/Runtime.h>

#include <Sprocket/Core/Window.h>
#include <Sprocket/Core/game_loop.h>

int main()
{
    spkt::window window("Runtime");
    Runtime runtime(&window);
    spkt::run_options options;
    options.show_frame_rate = true;
    return spkt::run(runtime, window, options);
}