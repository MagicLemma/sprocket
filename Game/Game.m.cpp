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
    ModelManager    modelManager;
    TextureManager  textureManager;
    MaterialManager materialManager(&textureManager);

    App app;
    
    window.SetCallback([&app](Event& event) {
        app.OnEvent(event);
    });

    CoreSystems core;
    core.window = &window;
    core.assetManager = &assetManager;
    core.modelManager = &modelManager;
    core.textureManager = &textureManager;
    core.materialManager = &materialManager;

    auto worldLayer = app.Add<WorldLayer>(core);
    auto editorUi = app.Add<EditorUI>(core, worldLayer.get());
    app.Add<EscapeMenu>(core, worldLayer.get());
 
    Stopwatch watch;

    Audio::Music sound;
    sound.Load("Resources/Audio/Sample.wav");
    sound.Play();

    while (window.Running()) {
        window.Clear();
        
        watch.OnUpdate();
        app.OnUpdate(watch.DeltaTime());

        std::string name = "Game! FPS: " + std::to_string(watch.Framerate());
        window.SetWindowName(name);
        window.OnUpdate();
    }

    return 0;
}