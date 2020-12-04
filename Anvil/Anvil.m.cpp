#include <Sprocket.h>
#include "EditorLayer.h"
#include "Adaptors.h"

int main()
{
    Sprocket::Window window("Workshop");
    Sprocket::EditorLayer editor(&window);
    return Sprocket::Run(editor, window);
}