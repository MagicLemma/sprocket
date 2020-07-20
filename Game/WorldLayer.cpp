#include "WorldLayer.h"
#include "Palette.h"
#include "PathFollower.h"
#include "PathCalculator.h"

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
    , d_entityManager({&d_selector, &d_scriptRunner, &d_pathFollower})
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
        auto worker = std::make_shared<Entity>();
        worker->Name() = "Worker";
        worker->Position() = {0.5f, 0.5f, 0.5f};
        
        worker->Add<SelectComponent>();

        auto path = worker->Add<PathComponent>();
        path->speed = 3.0f;
        path->markers.push({10.5f, 0.5f, 3.5f});

        auto modelData = worker->Add<ModelComponent>();
        modelData->model = ModelManager::LoadModel("Resources/Models/Cube.obj");
        modelData->scale = 0.5f;

        d_entityManager.AddEntity(worker);
        d_worker = worker.get();
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

    d_hoveredEntityUI.OnEvent(event);
    d_mouse.OnEvent(event);

    if (auto e = event.As<WindowResizeEvent>()) {
        d_postProcessor.SetScreenSize(e->Width(), e->Height());
    }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (e->Mods() & KeyModifier::CTRL) {
            Maths::vec3 cameraPos = d_camera->Position();
            Maths::vec3 direction = Maths::GetMouseRay(
                d_mouse.GetMousePos(),
                d_core.window->Width(),
                d_core.window->Height(),
                CameraUtils::MakeView(*d_camera),
                CameraUtils::MakeProj(*d_camera)
            );

            float lambda = -cameraPos.y / direction.y;
            Maths::vec3 mousePos = cameraPos + lambda * direction;
            mousePos.y = 0.5f;
            
            auto& path = d_worker->Get<PathComponent>();

            if (e->Button() == Mouse::LEFT) {
                std::queue<Maths::vec3>().swap(path.markers);
                if (Maths::Distance(d_worker->Position(), mousePos) > 1.0f) {
                    path.markers = GenerateAStarPath(
                        d_worker->Position(),
                        mousePos,
                        [&](const Maths::ivec2& pos) {
                            return d_gameGrid.At(pos.x, pos.y) != nullptr;
                        }
                    );
                } else {
                    path.markers.push(mousePos);
                }
                e->Consume();
            }
            else if (e->Button() == Mouse::RIGHT) {
                std::queue<Maths::vec3>().swap(path.markers);
                e->Consume();
            }
        }
    }

    d_entityManager.OnEvent(event);
    d_gameGrid.OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    using namespace Sprocket;
    Audio::SetListener(*d_camera);

    d_hoveredEntityUI.OnUpdate(dt);
    d_gameGrid.OnUpdate(d_core.window, d_camera);
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

    if (!d_paused) {
        d_hoveredEntityUI.StartFrame();

        auto mouse = d_mouse.GetMousePos();
        float w = (float)d_core.window->Width();
        float h = (float)d_core.window->Height();

        if (d_gameGrid.Hovered()) {
            float width = 200;
            float height = 50;
            float x = std::min(mouse.x - 5, w - width - 10);
            float y = std::min(mouse.y - 5, h - height - 10);

            Maths::vec4 region{x, y, width, height};
            bool active = true;
            bool draggable = false;
            bool clickable = false;
            if (d_hoveredEntityUI.StartPanel("Hovered", &region, &active, &draggable, &clickable)) {
                
                auto hovered = d_gameGrid.Hovered();
                d_hoveredEntityUI.Text(hovered->Name(), 36.0f, {0, 0, width, height});

                d_hoveredEntityUI.EndPanel();
            }
        }

        if (d_gameGrid.SelectedPosition().has_value()) {
            float width = 0.25f * w;
            float height = 0.6f * h;
            float x = (1.0f - 0.25f) * w;
            float y = ((1.0f - 0.6f) / 2) * h;

            static auto tex = Texture("Resources/Textures/BetterTree.png");

            Maths::vec4 region{x, y, width, height};
            bool active = true;
            bool draggable = false;
            bool clickable = true;
            if (d_hoveredEntityUI.StartPanel("Selected", &region, &active, &draggable, &clickable)) {
                
                auto selected = d_gameGrid.Selected();
                if (d_hoveredEntityUI.Button("Add Tree", {0, 0, width, 50})) {
                    d_gameGrid.DeleteSelected();

                    auto newEntity = std::make_shared<Entity>();
                    newEntity->Name() = "Tree";
                    //newEntity->Orientation() = Maths::Rotate({0, 1, 0}, d(gen));
                    auto modelData = newEntity->Add<ModelComponent>();
                    modelData->model = d_modelManager.GetModel("GG_Cube");
                    //modelData->scale = ef(gen);
                    modelData->scale = 1.0f;
                    modelData->material.texture = tex;
                    modelData->material.shineDamper = 10.0f;
                    modelData->material.reflectivity = 0.0f;
                    newEntity->Add<SelectComponent>();
                    d_entityManager.AddEntity(newEntity);

                    auto pos = d_gameGrid.SelectedPosition().value();
                    d_gameGrid.AddEntity(newEntity.get(), pos.x, pos.y);
                }

                if (d_hoveredEntityUI.Button("Add Rock", {0, 60, width, 50})) {
                    d_gameGrid.DeleteSelected();
                }

                if (d_hoveredEntityUI.Button("Clear", {0, 120, width, 50})) {
                    d_gameGrid.DeleteSelected();
                }

                d_hoveredEntityUI.EndPanel();
            }
        }

        d_hoveredEntityUI.EndFrame();
    }
}
