#include "WorldLayer.h"

void updatePhysics(const Sprocket::Entity& entity, float ts)
{
    using namespace Sprocket;
    if (!entity.hasComponent<PositionComponent>() && !entity.hasComponent<PhysicsComponent>()) {
        return;
    }

    auto& pos = entity.getComponent<PositionComponent>();
    auto& phy = entity.getComponent<PhysicsComponent>();

    pos.position += phy.velocity * ts;
    phy.velocity += phy.acceleration * ts;

    if(pos.position.y < 0) {
        pos.position.y = 0;
        phy.velocity = - 0.8f * phy.velocity;
    }
}

WorldLayer::WorldLayer(Sprocket::Accessor& accessor) 
    : Sprocket::Layer(accessor, Status::NORMAL, false)
    , d_entityRenderer(accessor.window())
    , d_terrainRenderer(accessor.window())
    , d_skyboxRenderer(accessor.window())
    , d_postProcessor(accessor.window()->width(), accessor.window()->height())
    , d_lens(accessor.window()->aspectRatio())
    , d_camera(&d_firstCamera)
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
{
    using namespace Sprocket;

    auto& entityManager = d_entityManager;

    Texture green("Resources/Textures/Green.PNG");
    Texture space("Resources/Textures/Space.PNG");
    Texture gray("Resources/Textures/PlainGray.PNG");

    Material dullGray;
    dullGray.texture = gray;

    Material shinyGray;
    shinyGray.texture = gray;
    shinyGray.reflectivity = 3.0f;
    shinyGray.shineDamper = 5.0f;

    Material field;
    field.texture = green;

    Material galaxy;
    galaxy.texture = space;

    d_terrains.push_back({field, {0.0f, 0.0f, 0.0f}});
    d_terrains.push_back({field, {-50.0f, 0.0f, 0.0f}});
    d_terrains.push_back({field, {0.0f, 0.0f, -50.0f}});
    d_terrains.push_back({field, {-50.0f, 0.0f, -50.0f}});

    auto dragon = Sprocket::Entity();
    auto model = dragon.addComponent<Sprocket::ModelComponent>();
    model->model = Sprocket::Model3D("Resources/Models/Dragon.obj");
    model->materials.push_back(shinyGray);

    auto pos = dragon.addComponent<Sprocket::PositionComponent>();
    pos->position = Maths::vec3{50.0f, 2.0f, 0.0f};
    pos->rotation = Maths::vec3{0.0f, 0.0f, 0.0f};
    pos->scale = 3.0f;

    auto phy = dragon.addComponent<Sprocket::PhysicsComponent>();
    phy->acceleration = {0.0f, -5.0f, 0.0f};
    phy->velocity = {0.0f, 10.0f, 0.0f};

    entityManager.push_back(dragon);

    accessor.window()->setCursorVisibility(false);

    d_lights.push_back({{0.0f, 50.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{-5.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.0f, 0.0f}});

    d_postProcessor.addEffect<GaussianVert>();
    d_postProcessor.addEffect<GaussianHoriz>();
}

bool WorldLayer::handleEventImpl(const Sprocket::Event& event)
{
    if (auto e = event.as<Sprocket::WindowResizeEvent>()) {
        d_postProcessor.setScreenSize(e->width(), e->height()); 
        SPKT_LOG_INFO("Resizing!");
    }

    d_camera->handleEvent(d_accessor.window(), event);
    d_lens.handleEvent(d_accessor.window(), event); 
      
    return false;
}

void WorldLayer::updateImpl()
{
    d_status = d_paused ? Status::PAUSED : Status::NORMAL;

    if (d_status == Status::NORMAL) {
        float tick = layerTicker();

        d_lights[1].position.z = 50.0f * std::sin(tick);
        d_lights[1].position.x = 50.0f * std::cos(tick);

        d_lights[2].position.z = 60.0f * std::sin(-1.5f * tick);
        d_lights[2].position.x = 60.0f * std::cos(-1.5f * tick);

        d_lights[3].position.z = 60.0f * std::sin(8.0f * tick);
        d_lights[3].position.x = 60.0f * std::cos(8.0f * tick);

        d_camera->update(d_accessor.window(), deltaTime());

        d_accessor.window()->setCursorVisibility(!d_cameraIsFirst);

        for (const auto& entity : d_entityManager) {
            updatePhysics(entity, deltaTime());
        }
    }
    else {
        d_accessor.window()->setCursorVisibility(true);
    }
    
}

void WorldLayer::drawImpl()
{
    if(d_paused) {
        d_postProcessor.bind();
    }
    
    Sprocket::RenderOptions options;
    options.wireframe = d_accessor.window()->isKeyDown(Sprocket::Keyboard::F) &&
                        d_status == Status::NORMAL;
    
    d_entityRenderer.update(*d_camera, d_lens, d_lights, options);
    d_terrainRenderer.update(*d_camera, d_lens, d_lights, options);

    for (const auto& entity: d_entityManager) {
        d_entityRenderer.draw(entity);
    }

    for (const auto& terrain: d_terrains) {
        d_terrainRenderer.draw(terrain);
    }
    d_skyboxRenderer.draw(d_skybox,
                          *d_camera,
                          d_lens);
    
    if (d_paused) {
        d_postProcessor.unbind();
        d_postProcessor.draw();
    }
}