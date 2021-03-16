#include <Sprocket.h>
#include "Anvil.h"

struct foo
{
    //std::any a;
    std::unique_ptr<void, void(*)(void*)> a;
    std::size_t x;
    std::string_view y;
    bool consumed;
};

int main()
{
    Sprocket::Window window("Anvil");
    Sprocket::Anvil editor(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(editor, window, options);
}
