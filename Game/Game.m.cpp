#include <Sprocket.h>
#include "WorldLayer.h"
#include "EditorUI.h"
#include "EscapeMenu.h"
#include "Palette.h"

int main()
{
    using namespace Sprocket;
    Log::Init();

    Window window("Game!");
    AssetManager    assetManager;

    App app;
    
    window.SetCallback([&app](Event& event) {
        app.OnEvent(event);
    });

    CoreSystems core;
    core.window = &window;
    core.assetManager = &assetManager;

    auto worldLayer = app.Add<WorldLayer>(&window);
    auto editorUi = app.Add<EditorUI>(core, worldLayer.get());
    app.Add<EscapeMenu>(core, worldLayer.get());
 
    Stopwatch watch;

    while (window.Running()) {
        window.Clear();
        
        double dt = watch.OnUpdate();
        app.OnUpdate(dt);
        app.OnRender();

        std::string name = "Game! FPS: " + std::to_string(watch.Framerate());
        window.SetWindowName(name);

        window.OnUpdate();
    }

    return 0;
}