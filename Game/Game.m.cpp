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

    App app;
    auto worldLayer = app.Add<WorldLayer>(&window);
    auto editorUi = app.Add<EditorUI>(worldLayer.get());
    app.Add<EscapeMenu>(worldLayer.get());
 
    Stopwatch watch;
    window.SetCallback([&app](Event& event) {
        app.OnEvent(event);
    });

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