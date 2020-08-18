#include <Sprocket.h>
#include "WorldLayer.h"
#include "EditorUI.h"
#include "EscapeMenu.h"

int main()
{
    Sprocket::Log::Init();
    Sprocket::Window window("Workshop");
    Sprocket::SceneManager sceneManager;
    Sprocket::ModelManager modelManager;
    Sprocket::TextureManager textureManager;

    window.SetCallback([&sceneManager](Sprocket::Event& event) {
        sceneManager.OnEvent(event);
    });

    Sprocket::CoreSystems core;
    core.window = &window;
    core.sceneManager = &sceneManager;
    core.modelManager = &modelManager;
    core.textureManager = &textureManager;

    auto world = sceneManager.Scene();
    auto worldLayer = world->Add<WorldLayer>(core);
    auto editorUi = world->Add<EditorUI>(core, worldLayer.get());
    world->Add<EscapeMenu>(core, worldLayer.get(), editorUi.get());

    Sprocket::Stopwatch watch;
    watch.PrintFramerate(false);

    while (window.Running()) {
        window.Clear();
        
        watch.OnUpdate();
        sceneManager.OnUpdate(watch.DeltaTime());

        window.OnUpdate();
    }

    return 0;
}