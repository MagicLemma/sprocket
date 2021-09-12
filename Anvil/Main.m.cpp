#include <Anvil/Anvil.h>

#include <Sprocket/Core/Window.h>
#include <Sprocket/Core/GameLoop.h>

int main()
{
    spkt::window window("Anvil");
    Anvil editor(&window);
    spkt::RunOptions options;
    options.showFramerate = true;
    return spkt::Run(editor, window, options);
}
