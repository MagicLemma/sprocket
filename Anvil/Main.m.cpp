#include <Sprocket.h>
#include "Anvil.h"

int main()
{
    spkt::Window window("Anvil");
    spkt::Anvil editor(&window);
    spkt::RunOptions options;
    options.showFramerate = true;
    return spkt::Run(editor, window, options);
}
