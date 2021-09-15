#include <Anvil/Anvil.h>

#include <Sprocket/Core/Window.h>
#include <Sprocket/Core/game_loop.h>

int main()
{
    spkt::window window("Anvil");
    Anvil editor(&window);
    spkt::run_options options;
    options.show_frame_rate = true;
    return spkt::run(editor, window, options);
}
