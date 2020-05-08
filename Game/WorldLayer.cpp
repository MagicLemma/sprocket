#include "WorldLayer.h"

WorldLayer::WorldLayer(Sprocket::Accessor& accessor) 
    : Sprocket::Layer(accessor, Status::NORMAL, false)
    , d_mode(Mode::OBSERVER)
    , d_entityRenderer(accessor.window())
    , d_terrainRenderer(accessor.window())
    , d_skyboxRenderer(accessor.window())
    , d_postProcessor(accessor.window()->width(), accessor.window()->height())
    , d_lens(accessor.window()->aspectRatio())
    , d_camera(&d_observerCamera)
    , d_skybox({
        Sprocket::Model3D("Resources/Models/Skybox.obj"),
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
    , d_playerMovement(accessor.window())
    , d_physicsEngine(Sprocket::Maths::vec3(0.0, -9.81, 0.0))
    , d_selector(accessor.window(), &d_editorCamera, &d_lens, &d_physicsEngine)
    , d_entityManager({&d_playerMovement, &d_physicsEngine, &d_selector})
{
    using namespace Sprocket;
    d_entityRenderer.wireFrame(false);
    d_entityRenderer.depthTest(true);
    d_entityRenderer.renderColliders(false);

    d_playerMovement.enable(false);

    auto& entityManager = d_entityManager;

    Texture green("Resources/Textures/Green.PNG");
    Texture space("Resources/Textures/Space.PNG");
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

    auto platformModel = Model3D("Resources/Models/Platform.obj");

    {
        auto platform = std::make_shared<Entity>();
        platform->position() = {7.0, 0.0, -3.0};
        platform->orientation() = Maths::rotate(Maths::mat3(1.0f), {1, 0, 0}, Maths::radians(6.0f));
        
        auto model = platform->add<ModelComponent>();
        model->model = platformModel;
        model->materials.push_back(dullGray);
        model->scale = 1.0f;
        auto phys = platform->add<PhysicsComponent>();
        phys->stationary = true;
        phys->bounciness = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        phys->collider = c;
        auto meta = platform->add<MetadataComponent>();
        meta->name = "Platform 1";
        platform->add<SelectComponent>();
        entityManager.addEntity(platform);
    }

    {
        auto platform = std::make_shared<Entity>();
        platform->position() = {-5.0, 0.0, 5.0};

        auto model = platform->add<ModelComponent>();
        model->model = platformModel;
        model->materials.push_back(dullGray);
        model->scale = 1.0f;
        auto phys = platform->add<PhysicsComponent>();
        phys->stationary = true;
        phys->bounciness = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        phys->collider = c;
        auto meta = platform->add<MetadataComponent>();
        meta->name = "Platform 2";
        platform->add<SelectComponent>();
        entityManager.addEntity(platform);
    }

    {
        auto platform = std::make_shared<Entity>();
        platform->position() = {-5.0, 0.0, 5.0};
        platform->orientation() = Maths::rotate(Maths::mat3(1.0f), {0, 0, 1}, Maths::radians(80.0f));
        platform->orientation() = Maths::rotate(platform->orientation(), {0, 1, 0}, Maths::radians(90.0f));
        
        auto model = platform->add<ModelComponent>();
        model->model = platformModel;
        model->materials.push_back(dullGray);
        model->scale = 1.0f;
        auto phys = platform->add<PhysicsComponent>();
        phys->stationary = true;
        phys->bounciness = 0.0f;
        phys->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {6.224951f, 0.293629f, 16.390110f};
        phys->collider = c;
        auto meta = platform->add<MetadataComponent>();
        meta->name = "Platform 3";
        platform->add<SelectComponent>();
        entityManager.addEntity(platform);
    }

    {
        auto crate = std::make_shared<Entity>();
        crate->position() = {-5.0, 2.0, -3.0};
        crate->orientation() = Maths::rotate(Maths::mat3(1.0f), {0, 1, 0}, Maths::radians(45.0f));
        
        auto model = crate->add<ModelComponent>();
        model->model = Model3D("Resources/Models/Cube.obj");
        model->materials.push_back(galaxy);
        model->scale = 1.2f;
        auto phys = crate->add<PhysicsComponent>();
        phys->stationary = true;
        phys->mass = 1000.0f;
        phys->bounciness = 0.0f;
        phys->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        phys->collider = c;
        auto meta = crate->add<MetadataComponent>();
        meta->name = "Crate 1";
        crate->add<SelectComponent>();
        entityManager.addEntity(crate);
    }

    {
        auto crate = std::make_shared<Entity>();
        crate->position() = {-1.0, 0.0, -3.0};
        crate->orientation() = Maths::rotate(Maths::mat3(1.0f), {0, 1, 0}, Maths::radians(75.0f));
        
        auto model = crate->add<ModelComponent>();
        model->model = Model3D("Resources/Models/Cube.obj");
        model->materials.push_back(field);
        model->scale = 1.2f;
        auto phys = crate->add<PhysicsComponent>();
        phys->stationary = true;
        phys->mass = 1000.0f;
        phys->bounciness = 0.0f;
        phys->frictionCoefficient = 0.0f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        phys->collider = c;
        auto meta = crate->add<MetadataComponent>();
        meta->name = "Crate 2";
        crate->add<SelectComponent>();
        entityManager.addEntity(crate);
    }

    {
        auto crate = std::make_shared<Entity>();
        crate->position() = {8.0, 5.0, 7.0};
        crate->orientation() = Maths::rotate(Maths::mat3(1.0f), {0, 1, 0}, Maths::radians(75.0f));
        
        auto model = crate->add<ModelComponent>();
        model->model = Model3D("Resources/Models/Cube.obj");
        model->materials.push_back(field);
        model->scale = 1.2f;
        auto phys = crate->add<PhysicsComponent>();
        phys->stationary = false;
        phys->mass = 10000.0f;
        phys->bounciness = 0.0f;
        phys->frictionCoefficient = 0.2f;
        BoxCollider c;
        c.halfExtents = {1.2, 1.2, 1.2};
        phys->collider = c;
        auto meta = crate->add<MetadataComponent>();
        meta->name = "Movable Crate";
        crate->add<SelectComponent>();
        entityManager.addEntity(crate);
    }

    {
        auto player = std::make_shared<Entity>();
        player->position() = {0.0f, 5.0f, 5.0f};
        player->orientation() = Maths::mat3(1.0f);
        
        auto model = player->add<ModelComponent>();
        model->model = Model3D("Resources/Models/Cube.obj");
        model->materials.push_back(shinyGray);
        model->scale = 0.3f;
        auto physics = player->add<PhysicsComponent>();
        physics->stationary = false;
        physics->mass = 60.0f;
        physics->rollingResistance = 1.0f;
        physics->frictionCoefficient = 0.4f;
        physics->bounciness = 0.0f;
        {
            CapsuleCollider c;
            c.radius = 0.5f;
            c.height = 1.0f;
            physics->collider = c;
        }
        player->add<PlayerComponent>();
        d_playerCamera.setPlayer(player.get());
        auto meta = player->add<MetadataComponent>();
        meta->name = "Player";
        player->add<SelectComponent>();
        entityManager.addEntity(player);
    }

    Model3D s("Resources/Models/Sphere.obj");
    for (int i = 0; i != 5; ++i)
    {
        auto sphere = std::make_shared<Entity>();
        sphere->position() = {0.0f, (float)i * 10.0f + 5.0f, 0.0f};
        sphere->orientation() = Maths::mat3(1.0f);
        
        auto model = sphere->add<ModelComponent>();
        model->model = s;
        model->materials.push_back(shinyGray);
        model->scale = 0.9f;
        auto physics = sphere->add<PhysicsComponent>();
        physics->stationary = false;
        physics->mass = 20.0f;
        {
            SphereCollider c;
            c.radius = 1;
            physics->collider = c;
        }
        auto meta = sphere->add<MetadataComponent>();
        std::stringstream ss;
        ss << "Sphere " << i;
        meta->name = ss.str();
        sphere->add<SelectComponent>();
        entityManager.addEntity(sphere);

        if (i == 4) {
            physics->velocity = {0, 20, 0};
        }
    }

    accessor.window()->setCursorVisibility(false);

    d_lights.push_back({{0.0f, 50.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{-7.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.0f, 0.0f}});

    d_postProcessor.addEffect<GaussianVert>();
    d_postProcessor.addEffect<GaussianHoriz>();
}

bool WorldLayer::handleEventImpl(const Sprocket::Event& event)
{
    using namespace Sprocket;

    if (auto e = event.as<WindowResizeEvent>()) {
        d_postProcessor.setScreenSize(e->width(), e->height()); 
        SPKT_LOG_INFO("Resizing!");
    }

    d_camera->handleEvent(d_accessor.window(), event);
    d_lens.handleEvent(d_accessor.window(), event);

    if (d_entityManager.handleEvent(event)) {
        return true;
    }
      
    return false;
}

void WorldLayer::updateImpl()
{
    using namespace Sprocket;
    d_status = d_paused ? Status::PAUSED : Status::NORMAL;

    if (d_status == Status::NORMAL) {
        d_camera->update(d_accessor.window(), deltaTime());
        d_accessor.window()->setCursorVisibility(d_mouseRequired);
        d_entityManager.update(deltaTime());

        for (auto& [id, entity] : d_entityManager.entities()) {
            if (entity->has<PlayerComponent>() && entity->position().y < -2.0f) {
                entity->position() = {0, 3, 0};
                entity->get<PhysicsComponent>().velocity = {0, 0, 0};
            }
            if (entity->position().y < -50.0f) {
                entity->kill();
            }
        }
    }
}

void WorldLayer::drawImpl()
{
    if(d_paused) {
        d_postProcessor.bind();
    }
    
    d_entityRenderer.wireFrame(
        d_accessor.window()->isKeyDown(Sprocket::Keyboard::F) &&
        d_status == Status::NORMAL);
    
    d_entityRenderer.update(*d_camera, d_lens, d_lights);

    d_skyboxRenderer.draw(d_skybox, *d_camera, d_lens);

    for (auto [id, entity]: d_entityManager.entities()) {
        d_entityRenderer.draw(*entity);
    }
    
    if (d_paused) {
        d_postProcessor.unbind();
        d_postProcessor.draw();
    }
}