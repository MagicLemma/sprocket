#include <Game/Game.h>

#include <Sprocket/Core/Window.h>
#include <Sprocket/Core/game_loop.h>

using namespace spkt;

int main()
{
    spkt::window window("Game");
    Game game(&window);
    spkt::run_options options;
    options.show_frame_rate = true;
    return spkt::run(game, window, options);
}
