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
    , d_entityManager({&d_selector, &d_scriptRunner})
    , d_gameGrid(&d_entityManager, &d_modelManager)
    , d_shadowMapRenderer(core.window)
    , d_hoveredEntityUI(core.window)
{
    using namespace Sprocket;

    SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_hoveredEntityUI.SetTheme(theme);

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
        gun->Position() = {10.0f, 1.5f, -3.0f};
        
        gun->Add<SelectComponent>();

        auto modelData = gun->Add<ModelComponent>();
        modelData->model = ModelManager::LoadModel("Resources/Models/Deagle.obj");
        
        d_entityManager.AddEntity(gun);
    }

    {
        auto camera = std::make_shared<Entity>();
        camera->Name() = "Camera";
        camera->Position() = {0, 5, 0};

        auto c = camera->Add<CameraComponent>();
      
        auto s = camera->Add<ScriptComponent>();
        s->script = "Resources/Scripts/ThirdPersonCamera.lua";

        d_camera = camera.get();
        d_entityManager.AddEntity(camera);
    }

    {
        auto terrain = std::make_shared<Entity>();
        terrain->Name() = "Terrain";
        terrain->Position() = {-25, 0, -25};

        auto modelData = terrain->Add<ModelComponent>();
        modelData->scale = 1.0f;
        modelData->model = MakeTerrain(51, 1.0f);
        modelData->material = GetTerrainMaterial();
        
        terrain->Add<SelectComponent>();

        d_entityManager.AddEntity(terrain);
    }
}

void WorldLayer::OnEvent(Sprocket::Event& event)
{
    using namespace Sprocket;
    d_mouse.OnEvent(event);

    if (auto e = event.As<WindowResizeEvent>()) {
        d_postProcessor.SetScreenSize(e->Width(), e->Height());
    }

    d_entityManager.OnEvent(event);
    d_gameGrid.OnEvent(event);
    d_hoveredEntityUI.OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    using namespace Sprocket;

    d_gameGrid.OnUpdate(d_core.window, d_camera);
    d_mouse.OnUpdate();
    d_cycle.OnUpdate(dt);
    d_hoveredEntityUI.OnUpdate(dt);

    Audio::SetListener(*d_camera);

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
        d_entityManager.OnUpdate(dt);
    }

    // Create the Shadow Map
    float lambda = 5.0f; // TODO: Calculate the floor intersection point
    Maths::vec3 target = d_camera->Position() + lambda * Maths::Forwards(d_camera->Orientation());
    d_shadowMapRenderer.BeginScene(d_lights.sun, target);
    for (const auto& [id, entity] : d_entityManager.Entities()) {
        d_shadowMapRenderer.Draw(*entity);
    }
    d_shadowMapRenderer.EndScene(); 

    if (d_paused) {
        d_postProcessor.Bind();
    }

    d_entityRenderer.BeginScene(*d_camera, d_lights);
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

    if (!d_paused && d_gameGrid.Selected()) {
        d_hoveredEntityUI.StartFrame();
        auto mouse = d_mouse.GetMousePos();

        float width = 200;
        float height = 50;
        float x = std::min(mouse.x, (float)(d_core.window->Width() - width - 10));
        float y = std::min(mouse.y, (float)(d_core.window->Height() - height - 10));

        Maths::vec4 region{x, y, width, height};
        bool active = true;
        bool draggable = false;
        if (d_hoveredEntityUI.StartPanel("Selected", &region, &active, &draggable)) {
            
            auto selected = d_gameGrid.Selected();
            d_hoveredEntityUI.Text(selected->Name(), 36.0f, {0, 0, width, height});

            d_hoveredEntityUI.EndPanel();
        }

        auto selected = d_gameGrid.Selected();

        d_hoveredEntityUI.EndFrame();
    }
}
