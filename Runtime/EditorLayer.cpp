#include "EditorLayer.h"

namespace Sprocket {
namespace {

std::string Name(const Entity& entity)
{
    if (entity.Has<NameComponent>()) {
        return entity.Get<NameComponent>().name;
    }
    return "Entity";
}

bool SubstringCI(const std::string& string, const std::string& substr) {
    auto it = std::search(
        string.begin(), string.end(),
        substr.begin(), substr.end(),
        [] (char c1, char c2) { return std::toupper(c1) == std::toupper(c2); }
    );
    return it != string.end();
}

}

EditorLayer::EditorLayer(const CoreSystems& core) 
    : d_core(core)
    , d_entityRenderer(core.assetManager)
    , d_skyboxRenderer(core.assetManager)
    , d_skybox({
        "Resources/Textures/Skybox/Skybox_X_Pos.png",
        "Resources/Textures/Skybox/Skybox_X_Neg.png",
        "Resources/Textures/Skybox/Skybox_Y_Pos.png",
        "Resources/Textures/Skybox/Skybox_Y_Neg.png",
        "Resources/Textures/Skybox/Skybox_Z_Pos.png",
        "Resources/Textures/Skybox/Skybox_Z_Neg.png"
    })
    , d_viewport(1280, 720)
    , d_ui(core.window)
{
    d_core.window->SetCursorVisibility(false);

    d_scene = std::make_shared<Scene>();    
    d_scene->AddSystem(std::make_shared<PhysicsEngine>(Maths::vec3{0.0, -9.81, 0.0}));
    d_scene->AddSystem(std::make_shared<CameraSystem>(d_core.window->AspectRatio()));
    d_scene->AddSystem(std::make_shared<ScriptRunner>());
    d_scene->AddSystem(std::make_shared<ParticleSystem>(&d_particleManager));
    d_scene->AddSystem(std::make_shared<AnimationSystem>());
    Loader::Load(d_sceneFile, d_scene);

    d_scene->OnStartup();

    d_scene->Each<CameraComponent>([&](Entity& entity) {
        d_runtimeCamera = entity;
    });

    d_activeScene = d_scene;
}

void EditorLayer::OnEvent(Event& event)
{
    if (auto e = event.As<WindowResizeEvent>()) {
        d_viewport.SetScreenSize(e->Width(), e->Height());
    }

    d_ui.OnEvent(event);
    d_activeScene->OnEvent(event);
}

void EditorLayer::OnUpdate(double dt)
{
    d_activeScene->OnUpdate(dt);
    d_particleManager.OnUpdate(dt);
    
    d_activeScene->Each<TransformComponent>([&](Entity& entity) {
        auto& transform = entity.Get<TransformComponent>();
        if (transform.position.y < -50) {
            entity.Kill();
        }
    });

    d_entityRenderer.EnableParticles(&d_particleManager);
    d_skyboxRenderer.Draw(d_skybox, d_runtimeCamera);
    d_entityRenderer.Draw(d_runtimeCamera, *d_activeScene);
}

}