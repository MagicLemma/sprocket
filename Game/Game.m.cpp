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
    ModelManager   modelManager;
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
    scene.Add<EscapeMenu>(core, worldLayer.get());
 
    Stopwatch watch;

    Audio::Music sound;
    sound.Load("Resources/Audio/Sample.wav");
    sound.Play();

    while (window.Running()) {
        window.Clear();
        
        watch.OnUpdate();
        scene.OnUpdate(watch.DeltaTime());

        std::string name = "Game! FPS: " + std::to_string(watch.Framerate());
        window.SetWindowName(name);
        window.OnUpdate();
    }

    return 0;
}