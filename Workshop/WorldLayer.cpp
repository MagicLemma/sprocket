#include "WorldLayer.h"

WorldLayer::WorldLayer(const Sprocket::CoreSystems& core) 
    : Sprocket::Layer(core)
    , d_mode(Mode::OBSERVER)
    , d_entityRenderer(core.window)
    , d_skyboxRenderer(core.window)
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
    , d_playerCamera(nullptr)
    , d_physicsEngine(Sprocket::Maths::vec3(0.0, -9.81, 0.0))
    , d_playerMovement()
    , d_selector(core.window, &d_physicsEngine)
    , d_entityManager({
        &d_playerMovement,
        &d_physicsEngine,
        &d_selector,
        &d_scriptRunner
    })
{
    using namespace Sprocket;

    d_playerMovement.Enable(false);

    auto& entityManager = d_entityManager;

    Texture green("Resources/Textures/Green.PNG");
    Texture space("Resources/Textures/Space.PNG");
    Texture spaceSpec("Resources/Textures/SpaceSpec.PNG");
    Texture gray("Resources/Textures/PlainGray.PNG");

    Material dullGray;
    dullGray.texture = gray;

    Material shinyGray;
    shinyGray.texture = gray;
    shinyGray.reflectivity = 2.0f;
    shinyGray.shineDamper = 3.0f;

    Material field;
    field.texture = green;

    Material galaxy;
    galaxy.texture = space;
    galaxy.specularMap = spaceSpec;

    Material islandMaterial;
    islandMaterial.texture = Texture("Resources/Textures/FloatingIslandTex.png");

    auto platformModel = core.modelManager->LoadModel("Platform", "Resources/Models/Platform.obj");
    auto crateModel = core.modelManager->LoadModel("Crate", "Resources/Models/Cube.obj");
    auto sphereModel = core.modelManager->LoadModel("Sphere", "Resources/Models/Sphere.obj");
    auto floatingIslandModel = core.modelManager->LoadModel("Floating Island", "Resources/Models/FloatingIsland.obj");

    {
        auto platform = std::make_shared<Entity>();
        platform->Name() = "Platform 1";
        platform->Position() = {7.0, 0.0, -3.0};
        platform->Orientation() = Maths::Rotate({1, 0, 0}, 6.0f);
        
        auto model = platform->Add<ModelComponent>();
        model->model = platformModel;
        model->material = dullGray;
        model->scale = 1.0f;

        auto phys = platform->Add<PhysicsComponent>();
        phys->bounciness = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        phys->collider = c;
        phys->frozen = true;

        platform->Add<SelectComponent>();
        entityManager.AddEntity(platform);
    }

    {
        auto platform = std::make_shared<Entity>();
        platform->Name() = "Island";
        platform->Position() = {40.0, -10.0, 0.0};
        
        auto model = platform->Add<ModelComponent>();
        model->model = floatingIslandModel;
        model->material = islandMaterial;
        model->scale = 0.5f;

        platform->Add<SelectComponent>();
        entityManager.AddEntity(platform);
    }

    {
        auto platform = std::make_shared<Entity>();
        platform->Name() = "Platform 2";
        platform->Position() = {-5.0, 0.0, 5.0};

        auto model = platform->Add<ModelComponent>();
        model->model = platformModel;
        model->material = dullGray;
        model->scale = 1.0f;

        auto phys = platform->Add<PhysicsComponent>();
        phys->bounciness = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        phys->collider = c;
        phys->frozen = true;

        platform->Add<SelectComponent>();
        entityManager.AddEntity(platform);
    }

    {
        auto platform = std::make_shared<Entity>();
        platform->Name() = "Platform 3";
        platform->Position() = {-5.0, 0.0, 5.0};

        Maths::quat orientation = Maths::identity;
        orientation = Maths::Rotate(orientation, {0, 0, 1}, 80.0f);
        orientation = Maths::Rotate(orientation, {0, 1, 0}, 90.0f);
        platform->Orientation() = orientation;

        auto model = platform->Add<ModelComponent>();
        model->model = platformModel;
        model->material = dullGray;
        model->scale = 1.0f;

        auto phys = platform->Add<PhysicsComponent>();
        phys->frozen = true;
        phys->bounciness = 0.0f;
        phys->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        phys->collider = c;

        platform->Add<SelectComponent>();
        entityManager.AddEntity(platform);
    }

    {
        auto crate = std::make_shared<Entity>();
        crate->Name() = "Crate 1";
        crate->Position() = {-5.0, 2.0, -3.0};
        crate->Orientation() = Maths::Rotate({0, 1, 0}, 45.0f);

        auto model = crate->Add<ModelComponent>();
        model->model = crateModel;
        model->material = galaxy;
        model->scale = 1.2f;

        auto phys = crate->Add<PhysicsComponent>();
        phys->frozen = true;
        phys->bounciness = 0.0f;
        phys->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        phys->collider = c;

        crate->Add<SelectComponent>();
        entityManager.AddEntity(crate);
    }

    {
        auto crate = std::make_shared<Entity>();
        crate->Name() = "Crate 2";
        crate->Position() = {-1.0, 0.0, -3.0};
        crate->Orientation() = Maths::Rotate({0, 1, 0}, 75.0f);

        auto model = crate->Add<ModelComponent>();
        model->model = crateModel;
        model->material = field;
        model->scale = 1.2f;

        auto phys = crate->Add<PhysicsComponent>();
        phys->frozen = true;
        phys->mass = 1000.0f;
        phys->bounciness = 0.0f;
        phys->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        phys->collider = c;

        crate->Add<SelectComponent>();
        entityManager.AddEntity(crate);
    }

    {
        auto crate = std::make_shared<Entity>();
        crate->Name() = "Movable Crate";
        crate->Position() = {8.0, 5.0, 7.0};
        crate->Orientation() = Maths::Rotate({0, 1, 0}, 75.0f);

        auto model = crate->Add<ModelComponent>();
        model->model = crateModel;
        model->material = field;
        model->scale = 1.2f;

        auto phys = crate->Add<PhysicsComponent>();
        phys->frozen = false;
        phys->mass = 10000.0f;
        phys->bounciness = 0.0f;
        phys->frictionCoefficient = 0.2f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        phys->collider = c;

        crate->Add<SelectComponent>();
        entityManager.AddEntity(crate);
    }

    {
        auto player = std::make_shared<Entity>();
        player->Name() = "Player";
        player->Position() = {0.0f, 5.0f, 5.0f};

        auto model = player->Add<ModelComponent>();
        model->model = crateModel;
        model->material = shinyGray;
        model->scale = 0.3f;

        auto phys = player->Add<PhysicsComponent>();
        phys->frozen = false;
        phys->mass = 60.0f;
        phys->rollingResistance = 1.0f;
        phys->frictionCoefficient = 0.4f;
        phys->bounciness = 0.0f;
        {
            CapsuleCollider c;
            c.radius = 0.5f;
            c.height = 1.0f;
            phys->collider = c;
        }

        player->Add<PlayerComponent>();

        auto c = player->Add<CameraComponent>();
        c->projection = Maths::Perspective(core.window->AspectRatio(), 70, 0.1f, 1000.0f);

        player->Add<SelectComponent>();

        d_playerCamera = player.get();
        entityManager.AddEntity(player);
    }

    {
        auto observerCamera = std::make_shared<Entity>();
        observerCamera->Name() = "Observer Camera";

        auto c = observerCamera->Add<CameraComponent>();
  
        auto s = observerCamera->Add<ScriptComponent>();
        s->script = "Resources/Scripts/FirstPersonCamera.lua";

        d_observerCamera = observerCamera.get();
        entityManager.AddEntity(observerCamera);
    }

    {
        auto editorCamera = std::make_shared<Entity>();
        editorCamera->Name() = "Editor Camera";
        editorCamera->Position() = {10.0f, 2.0f, 0.0f};

        auto c = editorCamera->Add<CameraComponent>();

        auto s = editorCamera->Add<ScriptComponent>();
        s->script = "Resources/Scripts/ThirdPersonCamera.lua";
        s->active = false;

        d_editorCamera = editorCamera.get();
        d_selector.SetCamera(d_editorCamera);
        entityManager.AddEntity(editorCamera);
    }

    for (int i = 0; i != 5; ++i)
    {
        auto sphere = std::make_shared<Entity>();
        std::stringstream ss;
        ss << "Sphere " << i;
        sphere->Name() = ss.str();
        sphere->Position() = {0.0f, (float)i * 10.0f + 5.0f, 0.0f};
        
        auto model = sphere->Add<ModelComponent>();
        model->model = sphereModel;
        model->material = shinyGray;
        model->scale = 0.9f;

        auto phys = sphere->Add<PhysicsComponent>();
        phys->mass = 20.0f;
        SphereCollider c;
        c.radius = 1;
        phys->collider = c;

        sphere->Add<SelectComponent>();
        entityManager.AddEntity(sphere);

        if (i == 4) {
            phys->velocity = {0, 20, 0};
        }
    }

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
    using namespace Sprocket;

    if (auto e = event.As<WindowResizeEvent>()) {
        d_postProcessor.SetScreenSize(e->Width(), e->Height());

        // We only do the player camera here as the observer and editor
        // projection matrices are updated via scripts.
        d_playerCamera->Get<CameraComponent>().projection =
            Maths::Perspective(e->AspectRatio(), 70, 0.1f, 1000.0f);
    }

    d_entityManager.OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    using namespace Sprocket;
    
    d_entityRenderer.BeginScene(*d_activeCamera, d_lights);

    if (!d_paused) {
        d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};
        d_core.window->SetCursorVisibility(d_mouseRequired);
        d_entityManager.OnUpdate(dt);

        for (auto& [id, entity] : d_entityManager.Entities()) {
            if (entity->Has<PlayerComponent>() && entity->Position().y < -2.0f) {
                entity->Position() = {0, 3, 0};
                entity->Get<PhysicsComponent>().velocity = {0, 0, 0};
            }
            if (entity->Position().y < -50.0f) {
                entity->Kill();
            }
        }
    }

    if (d_paused) {
        d_postProcessor.Bind();
    }

    d_skyboxRenderer.Draw(d_skybox, *d_activeCamera);
    d_entityManager.Draw(&d_entityRenderer);
    
    if (d_paused) {
        d_postProcessor.Unbind();
        d_postProcessor.Draw();
    }
}