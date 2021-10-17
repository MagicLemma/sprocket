#include <Game/game.h>

#include <Sprocket/Core/window.h>
#include <Sprocket/Core/game_loop.h>

using namespace spkt;

int main()
{
    spkt::window window("Game");
    app game(&window);
    spkt::run_options options;
    options.show_frame_rate = true;
    return spkt::run(game, window, options);
}
