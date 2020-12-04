#include <Sprocket.h>
#include "Runtime.h"

int main()
{
    Sprocket::Window window("Runtime");

    EditorLayer app(&window);
    return Sprocket::Run(app, window);
}