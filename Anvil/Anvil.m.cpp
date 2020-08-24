#include <Sprocket.h>
#include "EditorLayer.h"

int main()
{
    using namespace Sprocket;
    Log::Init();

    Window window("Workshop");
    ModelManager modelManager;
    TextureManager textureManager;

    App app;

    window.SetClearColour({0.5, 0.5, 0.5});
    window.SetCallback([&app](Event& event) {
        app.OnEvent(event);
    });

    CoreSystems core;
    core.window = &window;
    core.modelManager = &modelManager;
    core.textureManager = &textureManager;

    app.Add<EditorLayer>(core);

    Stopwatch watch;
    watch.PrintFramerate(false);

    while (window.Running()) {
        window.Clear();
        
        watch.OnUpdate();
        app.OnUpdate(watch.DeltaTime());

        window.OnUpdate();
    }

    return 0;
}