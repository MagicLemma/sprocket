#include "WorldLayer.h"

WorldLayer::WorldLayer(Sprocket::Accessor& accessor) 
    : Sprocket::Layer(accessor, Status::NORMAL, false)
    , d_entityRenderer(accessor.window())
    , d_terrainRenderer(accessor.window())
    , d_skyboxRenderer(accessor.window())
    , d_postProcessor(accessor.window()->width(), accessor.window()->height())
    , d_lens(accessor.window()->aspectRatio())
    , d_camera(&d_firstCamera)
    , d_skybox(Sprocket::Model3D("Resources/Models/Skybox.obj"),
             Sprocket::CubeMap(
                 {"Resources/Textures/Skybox/Skybox_X_Pos.png",
                 "Resources/Textures/Skybox/Skybox_X_Neg.png",
                 "Resources/Textures/Skybox/Skybox_Y_Pos.png",
                 "Resources/Textures/Skybox/Skybox_Y_Neg.png",
                 "Resources/Textures/Skybox/Skybox_Z_Pos.png",
                 "Resources/Textures/Skybox/Skybox_Z_Neg.png"}))
{
    using namespace Sprocket;

    auto& entityManager = d_entityManager;

    Texture green("Resources/Textures/Green.PNG");
    Texture space("Resources/Textures/Space.PNG");
    Texture gray("Resources/Textures/PlainGray.PNG");

    Material dullGray(gray);
    Material shinyGray(gray);
    Material field(green);
    Material galaxy(space);
    
    shinyGray.reflectivity(3);
    shinyGray.shineDamper(5);

    // Make the huge terrain.
    d_terrains.push_back({field, {0.0f, 0.0f, 0.0f}});
    d_terrains.push_back({field, {-50.0f, 0.0f, 0.0f}});
    d_terrains.push_back({field, {0.0f, 0.0f, -50.0f}});
    d_terrains.push_back({field, {-50.0f, 0.0f, -50.0f}});

    // Load complex models
    auto deagle = entityManager.addEntity();
    auto comp = deagle->addComponent<ModelComponent>();
    comp->model(Model3D("Resources/Models/Deagle.obj"));
    comp->addMaterial(shinyGray);

    auto pos = deagle->addComponent<PositionComponent>();
    pos->position(Maths::vec3{0.0f, 2.0f, 0.0f});
    pos->rotation(Maths::vec3{180.0f, 0.0f, 0.0f});
    pos->scale(1.0f);

    //auto dragon = entityManager.addEntity();
    //dragon->addComponent<Sprocket::ModelComponent>(
    //    Sprocket::Model3D("Resources/Models/Dragon.obj"),
    //    shinyGray
    //);
    //dragon->addComponent<Sprocket::PositionComponent>(
    //    Maths::vec3{50.0f, 2.0f, 0.0f},
    //    Maths::vec3{0.0f, 0.0f, 0.0f},
    //    3.0f
    //);

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> udist(-500, 500);
    std::uniform_real_distribution<float> urot(-180.0f, 180.0f);

    // Load a bunch of random cubes
    StaticBatcher sb("Resources/Models/Cube.obj");
    for (int i = 0; i != 50000; ++i) {
        sb.addTransform({udist(e1), 50 + udist(e1), udist(e1)},
                        {urot(e1), urot(e1), urot(e1)},
                        1.0f,
                        0.0f);
    }
    for (int i = 0; i != 50000; ++i) {
        sb.addTransform({udist(e1), 50 + udist(e1), udist(e1)},
                        {urot(e1), urot(e1), urot(e1)},
                        1.0f,
                        1.0f);
    }
    
    auto cubes = entityManager.addEntity();
    cubes->addComponent<ModelComponent>(
        sb.getModel3D(),
        shinyGray
    );


    // Load cubes to show the grid.
    //d_entities.push_back({"Resources/Models/Cube.obj", galaxy, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2f});
    //d_entities.push_back({"Resources/Models/Cube.obj", galaxy, {5.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2f});
    //d_entities.push_back({"Resources/Models/Cube.obj", galaxy, {10.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2f});
    //d_entities.push_back({"Resources/Models/Cube.obj", galaxy, {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, 0.2f});

    // Seed with a real random value, if available
    //std::random_device r;
    //std::default_random_engine e1(r());
    //std::uniform_real_distribution<float> udist(-50, 50);
    //std::uniform_real_distribution<float> urot(-180.0f, 180.0f);
//
    //// Load a bunch of random cubes/
    //for (int i = 0; i != 100; ++i) {
    //    d_entities.push_back({"Resources/Models/Cube.obj", shinyGray, {udist(e1), 50 + udist(e1), udist(e1)}, {urot(e1), urot(e1), urot(e1)}, 0.5f});
    //}

    accessor.window()->setCursorVisibility(false);

    d_lights.push_back({{0.0f, 50.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{-5.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
    d_lights.push_back({{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.0f, 0.0f}});

    //d_postProcessor.addEffect<Negative>();
    d_postProcessor.addEffect<GaussianVert>();
    d_postProcessor.addEffect<GaussianHoriz>();
}

bool WorldLayer::handleEventImpl(const Sprocket::Event& event)
{
    if (auto e = event.as<Sprocket::WindowResizeEvent>()) {
        d_postProcessor.setScreenSize(e->width(), e->height()); 
        SPKT_LOG_INFO("Resizing!");
    }

    if (d_entityManager.handleEvent(event)) {
        return true;
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

        d_entityManager.update();

        d_lights[1].position.z = 50.0f * std::sin(tick);
        d_lights[1].position.x = 50.0f * std::cos(tick);

        d_lights[2].position.z = 60.0f * std::sin(-1.5f * tick);
        d_lights[2].position.x = 60.0f * std::cos(-1.5f * tick);

        d_lights[3].position.z = 60.0f * std::sin(8.0f * tick);
        d_lights[3].position.x = 60.0f * std::cos(8.0f * tick);

        d_camera->update(d_accessor.window(), deltaTime());

        d_accessor.window()->setCursorVisibility(!d_cameraIsFirst);
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

    static Sprocket::Texture space("Resources/Textures/Space.PNG");
    for (auto entity: d_entityManager.entities()) {
        d_entityRenderer.draw(entity, space);
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