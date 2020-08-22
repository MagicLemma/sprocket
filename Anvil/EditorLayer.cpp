#include "EditorLayer.h"

namespace Sprocket {

WorldLayer::WorldLayer(const CoreSystems& core) 
    : Layer(core)
    , d_mode(Mode::OBSERVER)
    , d_entityRenderer(core.window, core.modelManager, core.textureManager)
    , d_skyboxRenderer(core.window)
    , d_postProcessor(core.window->Width(), core.window->Height())
    , d_skybox({
        ModelManager::LoadModel("Resources/Models/Skybox.obj"),
        CubeMap({
            "Resources/Textures/Skybox/Skybox_X_Pos.png",
            "Resources/Textures/Skybox/Skybox_X_Neg.png",
            "Resources/Textures/Skybox/Skybox_Y_Pos.png",
            "Resources/Textures/Skybox/Skybox_Y_Neg.png",
            "Resources/Textures/Skybox/Skybox_Z_Pos.png",
            "Resources/Textures/Skybox/Skybox_Z_Neg.png"
        })
    })
    , d_physicsEngine(Maths::vec3(0.0, -9.81, 0.0))
    , d_cameraSystem(core.window->AspectRatio())
    , d_selector(core.window, &d_physicsEngine)
    , d_scene({
        &d_physicsEngine,
        &d_selector,
        &d_cameraSystem,
        &d_scriptRunner
    })
    , d_serialiser(&d_scene)
{
    using namespace Sprocket;

    d_scene.OnStartup();

    d_serialiser.Deserialise("Resources/Anvil.yaml");

    d_scene.Each<CameraComponent>([&](Entity& entity) {
        d_camera = entity;
    });

    core.window->SetCursorVisibility(false);

    d_lights.points.push_back({{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{-7.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{40.0, 20.0, 0.0}, {0.8f, 0.8f, 0.8f}, {1.0f, 0.0f, 0.0f}});

    d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};
    d_lights.sun.colour = {1.0, 1.0, 1.0};
    d_lights.sun.brightness = 0.2f;

    d_postProcessor.AddEffect<GaussianVert>();
    d_postProcessor.AddEffect<GaussianHoriz>();
}

void WorldLayer::OnEvent(Event& event)
{
    if (auto e = event.As<WindowResizeEvent>()) {
        d_postProcessor.SetScreenSize(e->Width(), e->Height());
    }

    d_scene.OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    if (!d_paused) {
        d_scene.OnUpdate(dt);
        d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};
        d_core.window->SetCursorVisibility(d_mouseRequired);

        d_scene.Each<TransformComponent, PhysicsComponent>([&](Entity& entity) {
            auto& transform = entity.Get<TransformComponent>();
            auto& physics = entity.Get<PhysicsComponent>();
            
            if (entity.Has<CameraComponent>() && transform.position.y < -2) {
                transform.position = {0, 3, 0};
                physics.velocity = {0, 0, 0};
            }
            if (transform.position.y < -50) {
                entity.Kill();
            }
        });
    }

    d_entityRenderer.BeginScene(d_camera, d_lights);
    d_skyboxRenderer.Draw(d_skybox, d_camera);
    d_scene.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        d_entityRenderer.Draw(entity);
    });
}

}