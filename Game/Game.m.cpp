#include <Sprocket.h>
#include "WorldLayer.h"
#include "EditorUI.h"
#include "EscapeMenu.h"
#include "Palette.h"

void l(entt::registry&, entt::entity)
{
    SPKT_LOG_INFO("Added component");
}

int main()
{
    using namespace Sprocket;
    Log::Init();

    struct Temp
    {
        int x = 5;
    };

    struct X
    {
        int y = 5;

        void f(entt::registry&, entt::entity) {
            SPKT_LOG_INFO("{}", y);
        }        
    };

    NewScene scene;
    auto e = scene.CreateEntity();

    X x;

    scene.OnAdd<Temp, &X::f>(x);
    scene.OnAdd<Temp, &l>();
    scene.Add<Temp>(e);
    return 0;

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