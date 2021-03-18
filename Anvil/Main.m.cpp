#include <Sprocket.h>
#include "Anvil.h"


// See https://gist.github.com/zester/2438462 for better Lua integration

int main()
{
    Sprocket::Window window("Anvil");
    Sprocket::Anvil editor(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(editor, window, options);
}
