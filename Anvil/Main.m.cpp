#include <Sprocket.h>
#include "Anvil.h"

int main()
{
    Sprocket::Window window("Anvil");
    Sprocket::Anvil editor(&window);
    return Sprocket::Run(editor, window);
}
