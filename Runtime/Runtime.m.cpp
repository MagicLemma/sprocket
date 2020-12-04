#include <Sprocket.h>
#include "EditorLayer.h"

int main()
{
    Sprocket::Window window("Runtime");

    EditorLayer app(&window);
    return Sprocket::Run(app, window);
}