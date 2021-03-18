#include <Sprocket.h>
#include "Anvil.h"

int main()
{
    Sprocket::Window window("Anvil");
    Sprocket::Anvil editor(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(editor, window, options);
}
