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
    , d_mode(Mode::PLAYER)
    , d_entityRenderer(core.window)
    , d_terrainRenderer(core.window)
    , d_postProcessor(core.window->Width(), core.window->Height())
    , d_lens(core.window->AspectRatio())
    , d_entityManager({&d_selector})
    , d_terrain(GetTerrainMaterial())
    , d_sunAngle(0)
    , d_camera(5.0f)
{
    using namespace Sprocket;

    // Add the GameGrid square highligher to the ECS
    d_entityManager.AddEntity(d_gameGrid.GetHighlightSquare());

    d_sun.direction = {-Maths::Sind(d_sunAngle), -Maths::Cosd(d_sunAngle), 0.0f};
    d_sun.colour = {1.0, 1.0, 1.0};
    d_sun.brightness = 0.5f;

    d_postProcessor.AddEffect<GaussianVert>();
    d_postProcessor.AddEffect<GaussianHoriz>();
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

void WorldLayer::OnUpdate(float dt)
{
    using namespace Sprocket;
    
    d_entityRenderer.OnUpdate(d_camera, d_lens, d_sun);
    d_terrainRenderer.OnUpdate(d_camera, d_lens, d_sun);
    d_gameGrid.OnUpdate(d_core.window, &d_camera, &d_lens);
    d_mouse.OnUpdate();

    if (!d_paused) {
        d_sun.direction = {-Maths::Sind(d_sunAngle), -Maths::Cosd(d_sunAngle), 0.0f};
        d_camera.OnUpdate(dt);
        d_entityManager.OnUpdate(dt);
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