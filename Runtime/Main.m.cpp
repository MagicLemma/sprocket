#include <Runtime/Runtime.h>

#include <Sprocket/Core/Window.h>
#include <Sprocket/Core/GameLoop.h>

int main()
{
    spkt::window window("Runtime");
    Runtime runtime(&window);
    spkt::RunOptions options;
    options.showFramerate = true;
    return spkt::Run(runtime, window, options);
}