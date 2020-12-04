#include <Sprocket.h>
#include "EditorLayer.h"
#include "Adaptors.h"

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
    
    window.SetClearColour({0.5, 0.5, 0.5});
    window.SetCallback([&app](Event& event) {
        app.OnEvent(event);
    });

    GameLoop loop(&app, &window);
    loop.Run();

    return 0;
}