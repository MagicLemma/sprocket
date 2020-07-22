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
    SceneManager sceneManager;
    ModelManager modelManager;

    window.SetCallback([&sceneManager](Event& event) {
        sceneManager.OnEvent(event);
    });

    CoreSystems core;
    core.window = &window;
    core.sceneManager = &sceneManager;
    core.modelManager = &modelManager;

    auto world = sceneManager.AddScene("World");
    auto worldLayer = world->Add<WorldLayer>(core);
    auto editorUi = world->Add<EditorUI>(core, worldLayer.get());
    world->Add<EscapeMenu>(core, worldLayer.get());
    sceneManager.SetActiveScene("World");
 
    Stopwatch watch;

    Audio::Music sound;
    sound.Load("Resources/Audio/Sample.wav");
    sound.Play();

    while (window.Running()) {
        window.Clear();
        
        watch.OnUpdate();
        sceneManager.OnUpdate(watch.DeltaTime());

        std::string name = "Game! FPS: " + std::to_string(watch.Framerate());
        window.SetWindowName(name);
        window.OnUpdate();
    }

    return 0;
}