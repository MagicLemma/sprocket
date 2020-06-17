#include "WorldLayer.h"
#include "Palette.h"

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
    , d_mode(Mode::PLAYER)
    , d_entityRenderer(core.window)
    , d_postProcessor(core.window->Width(), core.window->Height())
    , d_lens(core.window->AspectRatio())
    , d_entityManager({&d_selector})
    , d_camera(5.0f)
    , d_gameGrid(&d_entityManager, &d_modelManager)
    , d_shadowMapRenderer(core.window)
{
    using namespace Sprocket;
    d_cycle.SetAngle(180.0f);

    d_lights.sun.direction = d_cycle.GetSunDir();
    d_lights.sun.colour = {1.0, 0.945, 0.789};
    d_lights.sun.brightness = 0.8f;

    d_lights.ambience.colour = {1.0, 1.0, 1.0};
    d_lights.ambience.brightness = 0.1f;

    d_postProcessor.AddEffect<GaussianVert>();
    d_postProcessor.AddEffect<GaussianHoriz>();

    {
        auto gun = std::make_shared<Entity>();
        gun->Name() = "Deagle";
        gun->Position() = {10.0f, -1.5f, -3.0f};
        gun->Orientation() = Maths::Rotate({1, 0, 0}, 180.0f);
        
        gun->Add<SelectComponent>();

        auto modelData = gun->Add<ModelComponent>();
        modelData->scale = 1.0f;
        modelData->model = ModelManager::LoadModel("Resources/Models/Deagle.obj");
        modelData->material.texture = Texture::White();
        
        d_entityManager.AddEntity(gun);
    }

    {
        auto terrain = std::make_shared<Entity>();
        terrain->Add<SelectComponent>();
        terrain->Name() = "Terrain";
        terrain->Position() = {-25, 0, -25};
        auto modelData = terrain->Add<ModelComponent>();
        modelData->scale = 1.0f;
        modelData->model = MakeTerrain(51, 1.0f);
        modelData->material = GetTerrainMaterial();
        //terrain->Position() = {0, 0, 0};
        
        d_entityManager.AddEntity(terrain);
    }
}

void WorldLayer::OnEvent(Sprocket::Event& event)
{
    using namespace Sprocket;
    d_mouse.OnEvent(event);

    if (auto e = event.As<WindowResizeEvent>()) {
        d_postProcessor.SetScreenSize(e->Width(), e->Height()); 
        SPKT_LOG_INFO("Resizing!");
    }

    d_camera.OnEvent(event);
    d_lens.OnEvent(event);
    d_entityManager.OnEvent(event);
    d_gameGrid.OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    using namespace Sprocket;

    d_gameGrid.OnUpdate(d_core.window, &d_camera, &d_lens);
    d_mouse.OnUpdate();
    d_cycle.OnUpdate(dt);   

    if (!d_paused) {
        float factor = (-d_cycle.GetSunDir().y + 1.0f) / 2.0f;
        float facSq = factor * factor;
        auto skyColour = (1.0f - facSq) * NAVY_NIGHT + facSq * LIGHT_BLUE;
        d_core.window->SetClearColour(skyColour);
        if (d_cycle.IsDay()) {
            d_lights.sun.direction = d_cycle.GetSunDir();
            d_lights.sun.colour = {1.0, 0.945, 0.789};
            d_lights.sun.brightness = 0.8f;
        }
        else {
            d_lights.sun.direction = -d_cycle.GetSunDir();
            d_lights.sun.colour = {0.5, 0.57, 0.98};
            d_lights.sun.brightness = 0.1f;
        }

        Maths::Normalise(d_lights.sun.direction);
        d_camera.OnUpdate(dt);
        d_entityManager.OnUpdate(dt);
    }

    // Create the Shadow Map
    d_shadowMapRenderer.BeginScene(d_lights.sun, d_camera.Target());
    for (const auto& [id, entity] : d_entityManager.Entities()) {
        d_shadowMapRenderer.Draw(*entity);
    }
    d_shadowMapRenderer.EndScene(); 

    if (d_paused) {
        d_postProcessor.Bind();
    }

    d_entityRenderer.BeginScene(d_camera, d_lens, d_lights);
    d_entityRenderer.EnableShadows(
        d_shadowMapRenderer.GetShadowMap(),
        d_shadowMapRenderer.GetLightProjViewMatrix()   
    );
    for (const auto& [id, entity] : d_entityManager.Entities()) {
        d_entityRenderer.Draw(*entity);
    }

    if (d_paused) {
        d_postProcessor.Unbind();
        d_postProcessor.Draw();
    }
}