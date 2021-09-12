#include <Game/Game.h>

#include <Sprocket/Core/Window.h>
#include <Sprocket/Core/GameLoop.h>

using namespace spkt;

int main()
{
    spkt::window window("Game");
    Game game(&window);
    spkt::RunOptions options;
    options.showFramerate = true;
    return spkt::Run(game, window, options);
}
