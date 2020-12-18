#include <Sprocket.h>
#include "Runtime.h"

int main()
{
    Sprocket::Window window("Runtime");
    Runtime runtime(&window);
    Sprocket::RunOptions options;
    options.showFramerate = true;
    return Sprocket::Run(runtime, window, options);
}