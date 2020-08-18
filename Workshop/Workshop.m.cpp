#include <Sprocket.h>
#include "WorldLayer.h"
#include "EditorUI.h"
#include "EscapeMenu.h"

int main()
{
    using namespace Sprocket;
    Log::Init();

    Window window("Workshop");
    ModelManager modelManager;
    TextureManager textureManager;

    Scene scene;

    window.SetCallback([&scene](Event& event) {
        scene.OnEvent(event);
    });

    CoreSystems core;
    core.window = &window;
    core.modelManager = &modelManager;
    core.textureManager = &textureManager;

    auto worldLayer = scene.Add<WorldLayer>(core);
    auto editorUi = scene.Add<EditorUI>(core, worldLayer.get());
    scene.Add<EscapeMenu>(core, worldLayer.get(), editorUi.get());

    Stopwatch watch;
    watch.PrintFramerate(false);

    while (window.Running()) {
        window.Clear();
        
        watch.OnUpdate();
        scene.OnUpdate(watch.DeltaTime());

        window.OnUpdate();
    }

    return 0;
}