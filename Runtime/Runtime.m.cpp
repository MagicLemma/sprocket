#include <Sprocket.h>
#include "EditorLayer.h"

int main()
{
    using namespace Sprocket;
    Log::Init();

    Window window("Workshop");
    AssetManager assetManager;

    CoreSystems core;
    core.window = &window;
    core.assetManager = &assetManager;

    EditorLayer app(core);
    return Run(app, window);
}