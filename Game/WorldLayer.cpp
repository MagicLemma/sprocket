#include "WorldLayer.h"
#include "Palette.h"
#include "PathFollower.h"
#include "PathCalculator.h"

namespace {

constexpr int ROWS = 50;
constexpr int COLUMNS = 50;

constexpr char MAP[ROWS + 1][COLUMNS + 1] = {
    "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
    "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr--rrrrrrrrr",
    "rrrrrrrrrrrrrr-----rrrrrrrrrrrrrrrrrr-----rrrrrrrr",
    "rrrrrrrrrr-------------rrrr-rrrr-rr----------rrrrr",
    "rrrr-------------------------------------------rrr",

    "rrrr--------------Trr-------T-----mmT---------rrrr",
    "rrrrr------------rrr----T--rrrmrrmr---m-------rrrr",
    "rrrrr----------------rrrrrrmrrrmr--mr---T------rrr",
    "rrrrr-------------Trrrrrrrrrrr----T-----------rrrr",
    "rrrrr----T------rrrrrrrrrr---rr----------------rrr",

    "rrrrr--Ti----------rrr------rr-----------------rrr",
    "rrrrr-------irrrr----------rrrrr----------------rr",
    "rrrrr----irrrri--T---------rrrrr----------------rr",
    "rrrrr------Tri---------------rrrr--------------rrr",
    "rrrr------------iT-----------rr----------------rrr",

    "rrrr------------------T---t---r----------------rrr",
    "rrrr--------------T-----trrr--rt--------------rrrr",
    "rrrrr----------------T--rrrrr-----------------rrrr",
    "rrrrr------------------trrrrrtr----------------rrr",
    "rrrrr--------------Trrrrrrrrtr-----------------rrr",

    "rrrrr------------------rrrrrr------------------rrr",
    "rrrrr----------------T--rrrt--------------------rr",
    "rrrrr---------------trr-T-t---------------------rr",
    "rrrr---------------T---------------------------rrr",
    "rrrr-------------------------------------------rrr",

    "rrrr--------------Trr-------T-----mmT---------rrrr",
    "rrr--------------rrr----T--rrrmrrmr---m-------rrrr",
    "rr-------------------rrrrrrmrrrmr--mr---T------rrr",
    "rr----------------Trrrrrrrrrrr----T-----------rrrr",
    "rr-------T------rrrrrrrrrr---rr----------------rrr",

    "rrr----Ti----------rrr------rr-----------------rrr",
    "rrr---------irrrr----------rrrrr----------------rr",
    "rrrr-----irrrri--T---------rrrrr----------------rr",
    "rrrr-------Tri---------------rrrr--------------rrr",
    "rrr-------------iT-----------rr----------------rrr",

    "rr---------------------------------------------rrr",
    "rr--------------------------------------------rrrr",
    "rr--------------------------------------------rrrr",
    "rr---------------------------------------------rrr",
    "rr---------------------------------------------rrr",

    "rrr----Ti----------rrr------rr-----------------rrr",
    "rrrr--------irrrr----------rrrrr----------------rr",
    "rrrr-----irrrri--T---------rrrrr----------------rr",
    "rrrr-------Tri---------------rrrr--------------rrr",
    "rrrr------------iT-----------rr----------------rrr",

    "rrrr-------------------------------------------rrr",
    "rrrrrrrrr--------------rrrrrrrrr-------------rrrrr",
    "rrrrrrrrrrrrr------rrrrrrrrrrrrrrr-------rrrrrrrrr",
    "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr--rrrrrrrrrrr",
    "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
};

}

WorldLayer::WorldLayer(const Sprocket::CoreSystems& core) 
    : Sprocket::Layer(core)
    , d_mode(Mode::PLAYER)
    //, d_entityRenderer(core.window, core.modelManager, core.textureManager)
    , d_postProcessor(core.window->Width(), core.window->Height())
    , d_renderer(core.window, core.modelManager, core.textureManager)
    , d_entityManager({&d_selector, &d_scriptRunner, &d_pathFollower, &d_renderer})
    , d_gameGrid(&d_entityManager)
    , d_shadowMapRenderer(core.window, core.modelManager, core.textureManager)
    , d_hoveredEntityUI(core.window)
{
    using namespace Sprocket;

    d_entityManager.OnStartup();

    SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_hoveredEntityUI.SetTheme(theme);

    d_cycle.SetAngle(190.0f);

    d_lights.sun.direction = d_cycle.GetSunDir();
    d_lights.sun.colour = {1.0, 0.945, 0.789};
    d_lights.sun.brightness = 0.8f;

    d_lights.ambience.colour = SARAWAK;
    d_lights.ambience.brightness = 0.4f;

    d_renderer.SetLights(d_lights);

    d_postProcessor.AddEffect<GaussianVert>();
    d_postProcessor.AddEffect<GaussianHoriz>();

    {
        auto worker = d_entityManager.NewEntity();

        auto& name = worker.Add<NameComponent>();
        name.name = "Worker";

        auto& tr = worker.Add<TransformComponent>();
        tr.position = {0.5f, 0.5f, 0.5f};
        
        worker.Add<SelectComponent>();

        auto& path = worker.Add<PathComponent>();
        path.speed = 3.0f;

        auto& modelData = worker.Add<ModelComponent>();
        modelData.model = "Resources/Models/Cube.obj";
        modelData.scale = 0.5f;

        d_worker = worker;
    }

    {
        auto camera = d_entityManager.NewEntity();

        auto& name = camera.Add<NameComponent>();
        name.name = "Camera";

        auto& tr = camera.Add<TransformComponent>();
        tr.position = {0, 5, 0};

        auto& c = camera.Add<CameraComponent>();
      
        ScriptComponent script;
        script.script = "Resources/Scripts/ThirdPersonCamera.lua";
        camera.Add<ScriptComponent>(script);

        d_camera = camera;
        d_renderer.SetCamera(d_camera);
    }

    {
        auto terrain = d_entityManager.NewEntity();

        auto& name = terrain.Add<NameComponent>();
        name.name = "Terrain";

        auto& tr = terrain.Add<TransformComponent>();
        tr.position = {0, 0, 0};

        auto& modelData = terrain.Add<ModelComponent>();
        modelData.scale = 25.0f;
        modelData.model = "Resources/Models/Square.obj";
        modelData.texture = "Resources/Textures/Green.PNG";
        
        terrain.Add<SelectComponent>();
    }

    for (int i = 0; i != ROWS; ++i) {
        for (int j = 0; j != COLUMNS; ++j) {
            char val = MAP[i][j];
            if (val == '-') {
                continue;
            }
            else if (val == 'T') {
                AddTree({i - 25, j - 25});
            }
            else if (val == 'r') {
                AddRock({i - 25, j - 25});
            }
            else if (val == 't') {
                AddTin({i - 25, j - 25});
            }
            else if (val == 'i') {
                AddIron({i - 25, j - 25});
            }
            else if (val == 'm') {
                AddMithril({i - 25, j - 25});
            }
        
        }
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
        auto& tr = d_camera.Get<TransformComponent>();
        if (e->Mods() & KeyModifier::CTRL) {
            Maths::vec3 cameraPos = tr.position;
            Maths::vec3 direction = Maths::GetMouseRay(
                d_mouse.GetMousePos(),
                d_core.window->Width(),
                d_core.window->Height(),
                CameraUtils::MakeView(d_camera),
                CameraUtils::MakeProj(d_camera)
            );

            float lambda = -cameraPos.y / direction.y;
            Maths::vec3 mousePos = cameraPos + lambda * direction;
            mousePos.y = 0.5f;
            
            auto& path = d_worker.Get<PathComponent>();

            if (e->Button() == Mouse::LEFT) {
                std::queue<Maths::vec3>().swap(path.markers);
                auto pos = d_worker.Get<TransformComponent>().position;
                if (Maths::Distance(pos, mousePos) > 1.0f) {
                    path.markers = GenerateAStarPath(
                        pos,
                        mousePos,
                        [&](const Maths::ivec2& pos) {
                            auto e = d_gameGrid.At(pos.x, pos.y);
                            return !e.Null();
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
    Audio::SetListener(d_camera);

    d_hoveredEntityUI.OnUpdate(dt);
    d_gameGrid.OnUpdate(d_core.window, &d_camera);
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
    //float lambda = 5.0f; // TODO: Calculate the floor intersection point
    //Maths::vec3 target = d_camera.Get<TransformComponent>().position + lambda * Maths::Forwards(d_camera.Get<TransformComponent>().orientation);
    //d_shadowMapRenderer.BeginScene(d_lights.sun, target);
    //d_entityManager.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
    //    d_shadowMapRenderer.Draw(entity);
    //});
    //d_shadowMapRenderer.EndScene(); 

    //if (d_paused) {
    //    d_postProcessor.Bind();
    //}
//
    //d_entityRenderer.BeginScene(d_camera, d_lights);
    //d_entityRenderer.EnableShadows(
    //    d_shadowMapRenderer.GetShadowMap(),
    //    d_shadowMapRenderer.GetLightProjViewMatrix()   
    //);
    //d_entityManager.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
    //    d_entityRenderer.Draw(entity);
    //});
//
    //if (d_paused) {
    //    d_postProcessor.Unbind();
    //    d_postProcessor.Draw();
    //}

    if (!d_paused) {
        d_hoveredEntityUI.StartFrame();

        auto mouse = d_mouse.GetMousePos();
        float w = (float)d_core.window->Width();
        float h = (float)d_core.window->Height();

        if (d_gameGrid.SelectedPosition().has_value()) {
            float width = 0.15f * w;
            float height = 0.6f * h;
            float x = w - width;
            float y = ((1.0f - 0.6f) / 2) * h;

            static auto ironTex = Texture("Resources/Textures/Iron.png");
            static auto tinTex = Texture("Resources/Textures/Tin.png");
            static auto mithrilTex = Texture("Resources/Textures/Mithril.png");

            Maths::vec4 region{x, y, width, height};
            bool active = true;
            bool draggable = false;
            bool clickable = true;
            if (d_hoveredEntityUI.StartPanel("Selected", &region, &active, &draggable, &clickable)) {
                
                auto selected = d_gameGrid.Selected();
                auto pos = d_gameGrid.SelectedPosition().value();
                if (d_hoveredEntityUI.Button("+Tree", {0, 0, width, 50})) {
                    d_gameGrid.DeleteSelected();
                    AddTree(pos);
                }

                if (d_hoveredEntityUI.Button("+Rock", {0, 60, width, 50})) {
                    d_gameGrid.DeleteSelected();
                    AddRock(pos);
                }

                if (d_hoveredEntityUI.Button("+Iron", {0, 120, width, 50})) {
                    d_gameGrid.DeleteSelected();
                    AddIron(pos);
                }

                if (d_hoveredEntityUI.Button("+Tin", {0, 180, width, 50})) {
                    d_gameGrid.DeleteSelected();
                    AddTin(pos);
                }

                if (d_hoveredEntityUI.Button("+Mithril", {0, 240, width, 50})) {
                    d_gameGrid.DeleteSelected();
                    AddMithril(pos);
                }

                if (d_hoveredEntityUI.Button("Clear", {0, 300, width, 50})) {
                    d_gameGrid.DeleteSelected();
                }

                d_hoveredEntityUI.EndPanel();
            }
        }


        auto hovered = d_gameGrid.Hovered();
        if (!hovered.Null()) {
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

                std::string name = "Unnamed";
                if (hovered.Has<NameComponent>()) {
                    name = hovered.Get<NameComponent>().name;
                }
                d_hoveredEntityUI.Text(name, 36.0f, {0, 0, width, height});

                d_hoveredEntityUI.EndPanel();
            }
        }

        d_hoveredEntityUI.EndFrame();
    }
}

void WorldLayer::AddTree(const Sprocket::Maths::ivec2& pos)
{
    using namespace Sprocket;
    static std::string tex = "Resources/Textures/BetterTree.png";

    auto newEntity = d_entityManager.NewEntity();

    auto& name = newEntity.Add<NameComponent>();
    name.name = "Tree";

    auto& tr = newEntity.Add<TransformComponent>();
    tr.orientation = Maths::Rotate({0, 1, 0}, Random(0.0f, 360.0f));

    auto& modelData = newEntity.Add<ModelComponent>();
    modelData.model = "Resources/Models/BetterTree.obj";
    modelData.scale = Random(1.0f, 1.3f);
    modelData.texture = tex;
    modelData.shineDamper = 10.0f;
    modelData.reflectivity = 0.0f;
    newEntity.Add<SelectComponent>();

    d_gameGrid.AddEntity(&newEntity, pos.x, pos.y);
}

void WorldLayer::AddRockBase(
    const Sprocket::Maths::ivec2& pos,
    const std::string& tex,
    const std::string& name)
{
    using namespace Sprocket;

    auto newEntity = d_entityManager.NewEntity();
    auto& n = newEntity.Add<NameComponent>();
    n.name = name;

    auto& tr = newEntity.Add<TransformComponent>();
    tr.position.y -= Random(0.0f, 0.5f);
    tr.orientation = Maths::Rotate({0, 1, 0}, 90 * Random(0, 3));

    auto& modelData = newEntity.Add<ModelComponent>();
    modelData.model = "Resources/Models/Rock.obj";
    modelData.scale = 1.1f;
    modelData.texture = tex;
    modelData.shineDamper = 10.0f;
    modelData.reflectivity = 0.0f;
    newEntity.Add<SelectComponent>();

    d_gameGrid.AddEntity(&newEntity, pos.x, pos.y);
}

void WorldLayer::AddRock(const Sprocket::Maths::ivec2& pos)
{
    static std::string tex = "Resources/Textures/Rock.png";
    AddRockBase(pos, tex, "Rock");
}

void WorldLayer::AddIron(const Sprocket::Maths::ivec2& pos)
{
    static std::string tex = "Resources/Textures/Iron.png";
    AddRockBase(pos, tex, "Iron");
}

void WorldLayer::AddTin(const Sprocket::Maths::ivec2& pos)
{
    static std::string tex = "Resources/Textures/Tin.png";
    AddRockBase(pos, tex, "Tin");
}

void WorldLayer::AddMithril(const Sprocket::Maths::ivec2& pos)
{
    static std::string tex = "Resources/Textures/Mithril.png";
    AddRockBase(pos, tex, "Mithril");
}