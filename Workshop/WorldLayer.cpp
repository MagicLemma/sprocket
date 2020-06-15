#include "WorldLayer.h"

WorldLayer::WorldLayer(const Sprocket::CoreSystems& core) 
    : Sprocket::Layer(core)
    , d_mode(Mode::OBSERVER)
    , d_entityRenderer(core.window)
    , d_skyboxRenderer(core.window)
    , d_postProcessor(core.window->Width(), core.window->Height())
    , d_lens(core.window->AspectRatio())
    , d_camera(&d_observerCamera)
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
    , d_selector(core.window, &d_editorCamera, &d_lens, &d_physicsEngine)
    , d_entityManager({&d_playerMovement, &d_physicsEngine, &d_selector})
    , d_observerCamera()
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

        auto coll = platform->Add<ColliderComponent>();
        coll->bounciness = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        coll->collider = c;

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

        auto coll = platform->Add<ColliderComponent>();
        coll->bounciness = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        coll->collider = c;

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

        auto coll = platform->Add<ColliderComponent>();
        coll->bounciness = 0.0f;
        coll->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        coll->collider = c;

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

        auto coll = crate->Add<ColliderComponent>();
        coll->bounciness = 0.0f;
        coll->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        coll->collider = c;

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

        auto coll = crate->Add<ColliderComponent>();
        coll->mass = 1000.0f;
        coll->bounciness = 0.0f;
        coll->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        coll->collider = c;

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

        auto coll = crate->Add<ColliderComponent>();
        coll->mass = 10000.0f;
        coll->bounciness = 0.0f;
        coll->frictionCoefficient = 0.2f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        coll->collider = c;

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

        auto physics = player->Add<PhysicsComponent>();

        auto coll = player->Add<ColliderComponent>();
        coll->mass = 60.0f;
        coll->rollingResistance = 1.0f;
        coll->frictionCoefficient = 0.4f;
        coll->bounciness = 0.0f;
        {
            CapsuleCollider c;
            c.radius = 0.5f;
            c.height = 1.0f;
            coll->collider = c;
        }

        player->Add<PlayerComponent>();
        d_playerCamera.SetPlayer(player.get());

        player->Add<SelectComponent>();
        entityManager.AddEntity(player);
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

        auto physics = sphere->Add<PhysicsComponent>();

        auto coll = sphere->Add<ColliderComponent>();
        coll->mass = 20.0f;
        SphereCollider c;
        c.radius = 1;
        coll->collider = c;

        sphere->Add<SelectComponent>();
        entityManager.AddEntity(sphere);

        if (i == 4) {
            physics->velocity = {0, 20, 0};
        }
    }

    core.window->SetCursorVisibility(false);

    d_lights.points.push_back({{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{-7.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.0f, 0.0f}});
    d_lights.points.push_back({{40.0, 20.0, 0.0}, {0.8f, 0.8f, 0.8f}, {1.0f, 0.0f, 0.0f}});

    d_lights.sun.direction = {-Maths::Sind(d_sunAngle), -Maths::Cosd(d_sunAngle), 0.0f};
    d_lights.sun.colour = {1.0, 1.0, 1.0};
    d_lights.sun.brightness = 0.2f;

    d_postProcessor.AddEffect<GaussianVert>();
    d_postProcessor.AddEffect<GaussianHoriz>();
}

void WorldLayer::OnEvent(Sprocket::Event& event)
{
    using namespace Sprocket;

    if (auto e = event.As<WindowResizeEvent>()) {
        d_postProcessor.SetScreenSize(e->Width(), e->Height()); 
        SPKT_LOG_INFO("Resizing!");
    }

    d_camera->OnEvent(event);
    d_lens.OnEvent(event);
    d_entityManager.OnEvent(event);
}

void WorldLayer::OnUpdate(float dt)
{
    using namespace Sprocket;
    
    d_entityRenderer.OnUpdate(
        *d_camera,
        d_lens,
        d_lights,
        0,
        Maths::mat4(1.0));

    if (!d_paused) {
        d_lights.sun.direction = {-Maths::Sind(d_sunAngle), -Maths::Cosd(d_sunAngle), 0.0f};
        d_camera->OnUpdate(dt);
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
    
    d_skyboxRenderer.Draw(d_skybox, *d_camera, d_lens);
    d_entityManager.Draw(&d_entityRenderer);
    
    if (d_paused) {
        d_postProcessor.Unbind();
        d_postProcessor.Draw();
    }
}