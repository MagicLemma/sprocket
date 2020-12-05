#include <Sprocket.h>
#include "Runtime.h"

int main()
{
    Sprocket::Window window("Runtime");
    Runtime runtime(&window);
    return Sprocket::Run(runtime, window);
}