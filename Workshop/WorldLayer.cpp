#include "WorldLayer.h"

WorldLayer::WorldLayer(const Sprocket::CoreSystems& core) 
    : Sprocket::Layer(core)
    , d_mode(Mode::OBSERVER)
    , d_skyboxRenderer(core.window)
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
    , d_physicsEngine(Sprocket::Maths::vec3(0.0, -9.81, 0.0))
    , d_playerMovement()
    , d_selector(core.window, &d_physicsEngine)
    , d_renderer(core.window, core.modelManager, core.textureManager)
    , d_entityManager({
        &d_playerMovement,
        &d_physicsEngine,
        &d_selector,
        &d_scriptRunner,
        &d_renderer
    })
{
    using namespace Sprocket;

    d_renderer.EnableShadows(false);
    d_entityManager.OnStartup();
    d_playerMovement.Enable(false);

    auto& entityManager = d_entityManager;

    std::string green = "Resources/Textures/Green.PNG";
    std::string space = "Resources/Textures/Space.PNG";
    std::string spaceSpec = "Resources/Textures/SpaceSpec.PNG";
    std::string gray = "Resources/Textures/PlainGray.PNG";
    std::string islandMaterial = "Resources/Textures/FloatingIslandTex.png";

    std::string platformModel = "Resources/Models/Platform.obj";
    std::string crateModel = "Resources/Models/Cube.obj";
    std::string sphereModel = "Resources/Models/Sphere.obj";
    std::string floatingIslandModel = "Resources/Models/FloatingIsland.obj";

    {
        auto platform = entityManager.NewEntity();

        auto& name = platform.Add<NameComponent>();
        name.name = "Platform 1";

        auto& tr = platform.Add<TransformComponent>();
        tr.position = {7.0, 0.0, -3.0};
        tr.orientation = Maths::Rotate({1, 0, 0}, 6.0f);
        
        auto& model = platform.Add<ModelComponent>();
        model.model = platformModel;
        model.texture = gray;
        model.scale = 1.0f;

        PhysicsComponent phys;
        phys.frozen = true;
        phys.collider = Collider::BOX;
        phys.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        platform.Add<PhysicsComponent>(phys);

        platform.Add<SelectComponent>();
    }

    {
        auto platform = entityManager.NewEntity();

        auto& name = platform.Add<NameComponent>();
        name.name = "Island";

        auto& tr = platform.Add<TransformComponent>();
        tr.position = {40.0, -10.0, 0.0};

        
        auto& model = platform.Add<ModelComponent>();
        model.model = floatingIslandModel;
        model.texture = islandMaterial;
        model.scale = 0.5f;

        platform.Add<SelectComponent>();
    }

    {
        auto platform = entityManager.NewEntity();
        auto& name = platform.Add<NameComponent>();
        name.name = "Platform 2";

        auto& tr = platform.Add<TransformComponent>();
        tr.position = {-5.0, 0.0, 5.0};


        auto& model = platform.Add<ModelComponent>();
        model.model = platformModel;
        model.texture = gray;
        model.scale = 1.0f;

        PhysicsComponent phys;
        phys.frozen = true;
        phys.bounciness = 0.0f;
        phys.collider = Collider::BOX;
        phys.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        platform.Add<PhysicsComponent>(phys);

        platform.Add<SelectComponent>();
    }

    {
        auto platform = entityManager.NewEntity();

        auto& name = platform.Add<NameComponent>();
        name.name = "Platform 3";

        auto& tr = platform.Add<TransformComponent>();
        tr.position = {-5.0, 0.0, 5.0};
        tr.orientation = Maths::Rotate(tr.orientation, {0, 0, 1}, 80.0f);
        tr.orientation = Maths::Rotate(tr.orientation, {0, 1, 0}, 90.0f);


        auto& model = platform.Add<ModelComponent>();
        model.model = platformModel;
        model.texture = gray;
        model.scale = 1.0f;

        PhysicsComponent phys;
        phys.frozen = true;
        phys.bounciness = 0.0f;
        phys.collider = Collider::BOX;
        phys.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        platform.Add<PhysicsComponent>(phys);

        platform.Add<SelectComponent>();
    }

    {
        auto crate = entityManager.NewEntity();

        auto& name = crate.Add<NameComponent>();
        name.name = "Crate";

        auto& tr = crate.Add<TransformComponent>();
        tr.position = {-5.0, 2.0, -3.0};
        tr.orientation = Maths::Rotate({0, 1, 0}, 45.0f);

        auto& model = crate.Add<ModelComponent>();
        model.model = crateModel;
        model.texture = space;
        model.scale = 1.2f;

        PhysicsComponent phys;
        phys.frozen = true;
        phys.bounciness = 0.0f;
        phys.frictionCoefficient = 0.0f;
        phys.collider = Collider::BOX;
        phys.halfExtents = {1.2, 1.2, 1.2};
        crate.Add<PhysicsComponent>(phys);

        crate.Add<SelectComponent>();
    }

    {
        auto crate = entityManager.NewEntity();

        auto& name = crate.Add<NameComponent>();
        name.name = "Crate 2";

        auto& tr = crate.Add<TransformComponent>();
        tr.position = {-1.0, 0.0, -3.0};
        tr.orientation = Maths::Rotate({0, 1, 0}, 75.0f);

        auto& model = crate.Add<ModelComponent>();
        model.model = crateModel;
        model.texture = green;
        model.scale = 1.2f;

        PhysicsComponent phys;
        phys.frozen = true;
        phys.mass = 1000.0f;
        phys.bounciness = 0.0f;
        phys.frictionCoefficient = 0.0f;
        phys.collider = Collider::BOX;
        phys.halfExtents = {1.2, 1.2, 1.2};
        crate.Add<PhysicsComponent>(phys);

        crate.Add<SelectComponent>();
    }

    {
        auto crate = entityManager.NewEntity();

        auto& name = crate.Add<NameComponent>();
        name.name = "Crate 3 (M)";

        auto& tr = crate.Add<TransformComponent>();
        tr.position = {8.0, 5.0, 7.0};
        tr.orientation = Maths::Rotate({0, 1, 0}, 75.0f);

        auto& model = crate.Add<ModelComponent>();
        model.model = crateModel;
        model.texture = green;
        model.scale = 1.2f;

        PhysicsComponent phys;
        phys.frozen = false;
        phys.mass = 10000.0f;
        phys.bounciness = 0.0f;
        phys.frictionCoefficient = 0.2f;
        phys.collider = Collider::BOX;
        phys.halfExtents = {1.2, 1.2, 1.2};
        crate.Add<PhysicsComponent>(phys);

        crate.Add<SelectComponent>();
    }

    {
        auto player = entityManager.NewEntity();

        auto& name = player.Add<NameComponent>();
        name.name = "Player";

        auto& tr = player.Add<TransformComponent>();
        tr.position = {0.0f, 5.0f, 5.0f};

        auto& model = player.Add<ModelComponent>();
        model.model = crateModel;
        model.texture = gray;
        model.scale = 0.3f;

        PhysicsComponent phys;
        phys.frozen = false;
        phys.mass = 60.0f;
        phys.rollingResistance = 1.0f;
        phys.frictionCoefficient = 0.4f;
        phys.bounciness = 0.0f;
        phys.collider = Collider::CAPSULE;
        phys.radius = 0.5f;
        phys.height = 1.0f;
        player.Add<PhysicsComponent>(phys);

        player.Add<PlayerComponent>();

        auto& c = player.Add<CameraComponent>();
        c.projection = Maths::Perspective(core.window->AspectRatio(), 70, 0.1f, 1000.0f);

        player.Add<SelectComponent>();

        d_playerCamera = player;
    }

    {
        auto observerCamera = entityManager.NewEntity();

        auto& name = observerCamera.Add<NameComponent>();
        name.name = "Observer Camera";

        observerCamera.Add<TransformComponent>();
        observerCamera.Add<CameraComponent>();
  
        ScriptComponent script;
        script.script = "Resources/Scripts/FirstPersonCamera.lua";
        observerCamera.Add<ScriptComponent>(script);

        d_observerCamera = observerCamera;
    }

    {
        auto editorCamera = entityManager.NewEntity();

        auto& name = editorCamera.Add<NameComponent>();
        name.name = "Editor Camera";

        auto& tr = editorCamera.Add<TransformComponent>();
        tr.position = {10.0f, 2.0f, 0.0f};

        editorCamera.Add<CameraComponent>();

        ScriptComponent script;
        script.script = "Resources/Scripts/ThirdPersonCamera.lua";
        script.active = false;
        editorCamera.Add<ScriptComponent>(script);

        d_editorCamera = editorCamera;
        d_selector.SetCamera(d_editorCamera);
    }

    for (int i = 0; i != 5; ++i)
    {
        auto sphere = entityManager.NewEntity();
        std::stringstream ss;
        ss << "Sphere " << i;

        auto& name = sphere.Add<NameComponent>();
        name.name = ss.str();

        auto& tr = sphere.Add<TransformComponent>();
        tr.position = {0.0f, (float)i * 10.0f + 5.0f, 0.0f};
        
        auto& model = sphere.Add<ModelComponent>();
        model.model = sphereModel;
        model.scale = 0.9f;
        model.texture = gray;
        model.reflectivity = 2.0f;
        model.shineDamper = 3.0f;

        PhysicsComponent phys;
        phys.mass = 20.0f;
        phys.collider = Collider::SPHERE;
        phys.radius = 1;
        sphere.Add<PhysicsComponent>(phys);

        sphere.Add<SelectComponent>();

        if (i == 4) {
            phys.velocity = {0, 20, 0};
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

    d_renderer.GetLights() = d_lights;
    d_renderer.AddEffect<GaussianVert>();
    d_renderer.AddEffect<GaussianHoriz>();

    if (d_mode == Mode::EDITOR) {
        d_activeCamera = d_editorCamera;
    }
    else if (d_mode == Mode::OBSERVER) {
        d_activeCamera = d_observerCamera;
    }
    else {
        d_activeCamera = d_playerCamera;
    }
    d_renderer.SetCamera(d_activeCamera);
}

void WorldLayer::OnEvent(Sprocket::Event& event)
{
    using namespace Sprocket;

    if (auto e = event.As<WindowResizeEvent>()) {
        // We only do the player camera here as the observer and editor
        // projection matrices are updated via scripts.
        d_playerCamera.Get<CameraComponent>().projection =
            Maths::Perspective(e->AspectRatio(), 70, 0.1f, 1000.0f);
    }

    d_entityManager.OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    using namespace Sprocket;
    
    if (!d_paused) {
        d_lights.sun.direction = {Maths::Sind(d_sunAngle), Maths::Cosd(d_sunAngle), 0.0f};
        d_core.window->SetCursorVisibility(d_mouseRequired);

        d_entityManager.Each<TransformComponent, PhysicsComponent>([&](Entity& entity) {
            auto& transform = entity.Get<TransformComponent>();
            auto& physics = entity.Get<PhysicsComponent>();
            
            if (entity.Has<PlayerComponent>() && transform.position.y < -2) {
                transform.position = {0, 3, 0};
                physics.velocity = {0, 0, 0};
            }
            if (transform.position.y < -50) {
                entity.Kill();
            }
        });
    }

    d_skyboxRenderer.Draw(d_skybox, d_activeCamera);
    d_renderer.EnablePostProcessor(d_paused);
    d_entityManager.OnUpdate(dt, !d_paused);
}