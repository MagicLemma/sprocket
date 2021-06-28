#include <Sprocket.h>
#include "Runtime.h"

int main()
{
    spkt::Window window("Runtime");
    Runtime runtime(&window);
    spkt::RunOptions options;
    options.showFramerate = true;
    return spkt::Run(runtime, window, options);
}