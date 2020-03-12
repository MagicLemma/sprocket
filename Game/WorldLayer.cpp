#include "WorldLayer.h"

WorldLayer::WorldLayer(std::shared_ptr<BasicSceneInfo> info) 
    : Sprocket::Layer(Status::NORMAL, false) 
    , d_info(info)
    , d_entityRenderer(info->window)
    , d_terrainRenderer(info->window)
    , d_skyboxRenderer(info->window)
    , d_postProcessor(info->window->width(), info->window->height())
{
    auto quadModel = Sprocket::Loader::loadModel3D("Resources/Models/Plane.obj");
    auto dragonModel = Sprocket::Loader::loadModel3D("Resources/Models/Dragon.obj");
    auto deagleModel = Sprocket::Loader::loadModel3D("Resources/Models/Deagle.obj");
    auto cube = Sprocket::Loader::loadModel3D("Resources/Models/Cube.obj");

    auto green = Sprocket::Loader::loadTexture("Resources/Textures/Green.PNG");
    auto space = Sprocket::Loader::loadTexture("Resources/Textures/Space.PNG");
    auto gray = Sprocket::Loader::loadTexture("Resources/Textures/PlainGray.PNG");
    auto shinyGray = Sprocket::Loader::loadTexture("Resources/Textures/PlainGray.PNG");
    
    shinyGray.reflectivity(3);
    shinyGray.shineDamper(5);

    // Make the huge terrain.
    d_info->terrains.push_back(Sprocket::Terrain(green, {0.0f, 0.0f, 0.0f}));
    d_info->terrains.push_back(Sprocket::Terrain(green, {-50.0f, 0.0f, 0.0f}));
    d_info->terrains.push_back(Sprocket::Terrain(green, {0.0f, 0.0f, -50.0f}));
    d_info->terrains.push_back(Sprocket::Terrain(green, {-50.0f, 0.0f, -50.0f}));

    // Load complex models
    d_info->entities.push_back(Sprocket::Entity(dragonModel, shinyGray, {0.0f, 0.0f, -1.0f}, Sprocket::Maths::vec3(0.0f), 0.1f));
    d_info->entities.push_back(Sprocket::Entity(deagleModel, shinyGray, {0.0f, 2.0f, 0.0f}, {180.0f, 0.0f, 0.0f}, 1));
    
    // Load cubes to show the grid.
    d_info->entities.push_back(Sprocket::Entity(cube, space, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2));
    d_info->entities.push_back(Sprocket::Entity(cube, space, {5.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2));
    d_info->entities.push_back(Sprocket::Entity(cube, space, {10.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2));
    d_info->entities.push_back(Sprocket::Entity(cube, space, {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, 0.2));

    // Seed with a real random value, if available
    std::random_device r;

    // Choose a random mean between 1 and 6
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> udist(-50, 50);
    std::uniform_real_distribution<float> urot(-180.0f, 180.0f);

    // Load a bunch of random cubes/
    for (int i = 0; i != 1000; ++i) {
        d_info->entities.push_back(Sprocket::Entity{cube, shinyGray, {udist(e1), 50 + udist(e1), udist(e1)}, {urot(e1), urot(e1), urot(e1)}, 0.5f});
    }

    d_info->window->setCursorVisibility(false);

    d_info->lights.push_back(Sprocket::Light{{0.0f, 50.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {1.0f, 0.0f, 0.0f}});
    d_info->lights.push_back(Sprocket::Light{{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    d_info->lights.push_back(Sprocket::Light{{-5.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
    d_info->lights.push_back(Sprocket::Light{{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.0f, 0.0f}});

    //d_postProcessor.addEffect<Negative>();
    d_postProcessor.addEffect<Sprocket::GaussianVert>();
    d_postProcessor.addEffect<Sprocket::GaussianHoriz>();
}

bool WorldLayer::handleEventImpl(Sprocket::Window* window, const Sprocket::Event& event)
{
    if (auto e = event.as<Sprocket::WindowResizeEvent>()) {
        d_postProcessor.setScreenSize(e->width(), e->height());
        d_info->camera.handleEvent(window, event);
        d_info->lens.handleEvent(window, event);  
        SPKT_LOG_INFO("Resizing!");
    }

      
    return false;
}

void WorldLayer::updateImpl(Sprocket::Window* window)
{
    d_status = d_info->paused ? Status::PAUSED : Status::NORMAL;

    if (d_status == Status::NORMAL) {
        double tick = layerTicker();

        d_info->lights[1].position.z = 50 * std::sin(tick);
        d_info->lights[1].position.x = 50 * std::cos(tick);

        d_info->lights[2].position.z = 60 * std::sin(-1.5f * tick);
        d_info->lights[2].position.x = 60 * std::cos(-1.5f * tick);

        d_info->lights[3].position.z = 60 * std::sin(8.0f * tick);
        d_info->lights[3].position.x = 60 * std::cos(8.0f * tick);

        d_info->camera.update(window, deltaTime());

        window->setCursorVisibility(false);
    }
    else {
        window->setCursorVisibility(true);
    }
    
}

void WorldLayer::drawImpl(Sprocket::Window* window)
{
    if(d_info->paused) {
        d_postProcessor.bind();
    }
    
    Sprocket::RenderOptions options;
    options.wireframe = window->isKeyDown(Sprocket::Keyboard::F) &&
                        d_status == Status::NORMAL;

    
    d_entityRenderer.update(d_info->camera, d_info->lens, d_info->lights, options);
    d_terrainRenderer.update(d_info->camera, d_info->lens, d_info->lights, options);

    for (const auto& entity: d_info->entities) {
        d_entityRenderer.draw(entity);
    }

    for (const auto& terrain: d_info->terrains) {
        d_terrainRenderer.draw(terrain);
    }
    d_skyboxRenderer.draw(d_info->skybox,
                            d_info->camera,
                            d_info->lens);
    
    
    if (d_info->paused) {
        d_postProcessor.unbind();
        d_postProcessor.draw();
    }
}