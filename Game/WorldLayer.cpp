#include "WorldLayer.h"

namespace {

Sprocket::Material GetTerrainMaterial()
{
    Sprocket::Material dullGray;
    dullGray.texture = Sprocket::Texture("Resources/Textures/Green.PNG");
    return dullGray;
}

}

WorldLayer::WorldLayer(const Sprocket::CoreSystems& core) 
    : Sprocket::Layer(core)
    , d_mode(Mode::OBSERVER)
    , d_entityRenderer(core.window)
    , d_terrainRenderer(core.window)
    , d_postProcessor(core.window->Width(), core.window->Height())
    , d_lens(core.window->AspectRatio())
    , d_camera(&d_observerCamera)
    , d_entityManager({&d_selector})
    , d_observerCamera()
    , d_terrain(GetTerrainMaterial())
{
    using namespace Sprocket;

    auto& entityManager = d_entityManager;

    Texture green("Resources/Textures/Green.PNG");
    Texture space("Resources/Textures/Space.PNG");
    Texture spaceSpec("Resources/Textures/SpaceSpec.PNG");

    auto crateModel = core.modelManager->LoadModel("Crate", "Resources/Models/Cube.obj");

    Material field;
    field.texture = green;

    {
        auto crate = std::make_shared<Entity>();
        crate->Name() = "Crate";
        crate->Position() = {0.0, 0.0, 0.0};
        crate->Add<SelectComponent>();

        auto model = crate->Add<ModelComponent>();
        model->model = crateModel;
        model->material = field;
        model->scale = 1.2f;

        entityManager.AddEntity(crate);
    }
    
    core.window->SetCursorVisibility(false);

    d_sun.direction = {-Maths::Sind(d_sunAngle), -Maths::Cosd(d_sunAngle), 0.0f};
    d_sun.colour = {1.0, 1.0, 1.0};
    d_sun.brightness = 0.2f;

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
    
    d_entityRenderer.OnUpdate(*d_camera, d_lens, d_sun);
    d_terrainRenderer.OnUpdate(*d_camera, d_lens, d_sun);

    if (!d_paused) {
        d_sun.direction = {-Maths::Sind(d_sunAngle), -Maths::Cosd(d_sunAngle), 0.0f};
        d_core.window->SetCursorVisibility(d_mouseRequired);

        d_camera->OnUpdate(dt);
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
    
    d_entityManager.Draw(&d_entityRenderer);
    d_terrainRenderer.Draw(d_terrain);
    
    if (d_paused) {
        d_postProcessor.Unbind();
        d_postProcessor.Draw();
    }
}