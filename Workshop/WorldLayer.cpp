#include "WorldLayer.h"

WorldLayer::WorldLayer(const Sprocket::CoreSystems& core) 
    : Sprocket::Layer(core)
    , d_mode(Mode::OBSERVER)
    , d_entityRenderer(core.window, core.modelManager, core.textureManager)
    , d_skyboxRenderer()
    , d_postProcessor(core.window->Width(), core.window->Height())
    , d_skybox({
        Sprocket::ModelManager::LoadModel("Resources/Models/Skybox.obj"),
        Sprocket::CubeMap({
            "Resources/Textures/Skybox/Skybox_X_Pos.png",
            "Resources/Textures/Skybox/Skybox_X_Neg.png",
            "Resources/Textures/Skybox/Skybox_Y_Pos.png",
            "Resources/Textures/Skybox/Skybox_Y_Neg.png",
            "Resources/Textures/Skybox/Skybox_Z_Pos.png",
            "Resources/Textures/Skybox/Skybox_Z_Neg.png"
        })
    })
    , d_physicsEngine(std::make_shared<Sprocket::PhysicsEngine>(Sprocket::Maths::vec3(0.0, -9.81, 0.0)))
    , d_cameraSystem(std::make_shared<Sprocket::CameraSystem>(core.window->AspectRatio()))
    , d_selector(std::make_shared<Sprocket::Selector>(core.window, d_physicsEngine.get()))
    , d_scriptRunner(std::make_shared<Sprocket::ScriptRunner>())
{
    using namespace Sprocket;

    d_scene = std::make_shared<Scene>();
    d_scene->AddSystem(d_physicsEngine);
    d_scene->AddSystem(d_selector);
    d_scene->AddSystem(d_cameraSystem);
    d_scene->AddSystem(d_scriptRunner);

    Loader::Load("Resources/WorkshopScene.yaml", d_scene);
    d_scene->OnStartup();

    d_scene->Each<NameComponent>([&](Entity& entity) {
        const auto& name = entity.Get<NameComponent>().name;
        if (name == "Editor Camera") {
            d_editorCamera = entity;
            d_selector->SetCamera(entity);
        }
        else if (name == "Observer Camera") {
            d_observerCamera = entity;
        }
        else if (name == "Player") {
            d_playerCamera = entity;
        }
    });

    core.window->SetCursorVisibility(false);

    d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};
    d_lights.sun.colour = {1.0, 1.0, 1.0};
    d_lights.sun.brightness = 0.2f;

    d_postProcessor.AddEffect<GaussianVert>();
    d_postProcessor.AddEffect<GaussianHoriz>();

    if (d_mode == Mode::EDITOR) {
        d_activeCamera = d_editorCamera;
    }
    else if (d_mode == Mode::OBSERVER) {
        d_activeCamera = d_observerCamera;
    }
    else {
        d_activeCamera = d_playerCamera;
    }
}

void WorldLayer::OnEvent(Sprocket::Event& event)
{
    if (auto e = event.As<Sprocket::WindowResizeEvent>()) {
        d_postProcessor.SetScreenSize(e->Width(), e->Height());
    }

    d_scene->OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    using namespace Sprocket;
    

    if (!d_paused) {
        d_scene->OnUpdate(dt);
        d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};
        d_core.window->SetCursorVisibility(d_mouseRequired);

        d_scene->Each<TransformComponent, RigidBody3DComponent>([&](Entity& entity) {
            auto& transform = entity.Get<TransformComponent>();
            auto& physics = entity.Get<RigidBody3DComponent>();
            
            if (entity.Has<CameraComponent>() && transform.position.y < -2) {
                transform.position = {0, 3, 0};
                physics.velocity = {0, 0, 0};
            }
            if (transform.position.y < -50) {
                entity.Kill();
            }
        });
    }

    if (d_paused) {
        d_postProcessor.Bind();
    }

    d_entityRenderer.Draw(d_activeCamera, d_lights, *d_scene);
    d_skyboxRenderer.Draw(d_skybox, d_activeCamera);
    
    if (d_paused) {
        d_postProcessor.Unbind();
        d_postProcessor.Draw();
    }
}