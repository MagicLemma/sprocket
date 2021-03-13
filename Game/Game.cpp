#include "Game.h"
#include "Palette.h"
#include "PathFollower.h"
#include "PathCalculator.h"
#include "Window.h"

#include <cmath>

using namespace Sprocket;

namespace {

void SunInfoPanel(DevUI& ui, CircadianCycle& cycle)
{
    ImGui::Begin("Sun");

    if (ImGui::Button("Start")) { cycle.Start(); }
    ImGui::SameLine();
    if (ImGui::Button("Stop")) { cycle.Stop(); }
    ImGui::Separator();

    if (cycle.IsRunning()) { // If running, be able to change the speed.
        float speed = cycle.GetSpeed();
        ImGui::SliderFloat("Speed", &speed, 1.0f, 1000.0f, "%.3f");
        cycle.SetSpeed(speed);
    }
    else { // If not running, be able to set manually
        float angle = cycle.GetAngle();
        ImGui::DragFloat("Angle", &angle, 0.01f);
        cycle.SetAngle(angle);
    }
    
    ImGui::Separator();
    ImGuiXtra::Text(cycle.ToString12Hour());
    ImGui::End();
}

void ShaderInfoPanel(DevUI& ui, Shader& shader)
{
    static std::string compileStatus;

    ImGui::Begin("Shader");
    if(ImGui::Button("Recompile")) {
        bool result = shader.Reload();
        compileStatus ="Shader compile:";
        compileStatus += (result? " SUCCESS": " FAILURE");
    }

    ImGui::Text(compileStatus.c_str());
    
    bool closed = true;
    if (ImGui::CollapsingHeader("Vertex")) {
        closed = false;
        ImGuiXtra::MultilineTextModifiable("", &shader.VertexShaderSource());
    }
    if (ImGui::CollapsingHeader("Frag")) {
        closed = false;
        ImGuiXtra::MultilineTextModifiable("", &shader.FragShaderSource());
    }
    
    if(closed) {
        compileStatus.clear();
    }

    ImGui::End();
}

}

WorldLayer::WorldLayer(Window* window) 
    : d_window(window)
    , d_assetManager()
    , d_mode(Mode::PLAYER)
    , d_entityRenderer(&d_assetManager)
    , d_postProcessor(d_window->Width(), d_window->Height())
    , d_shadowMap(&d_assetManager)
    , d_hoveredEntityUI(d_window)
    , d_devUI(window)
    , d_escapeMenu(window)
{
    using namespace Sprocket;

    LoadScene("Resources/Scene.yaml");

    SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_hoveredEntityUI.SetTheme(theme);
    d_escapeMenu.SetTheme(theme);    

    d_cycle.SetAngle(3.14195f);

    auto& sun = d_scene.Entities().Find<SunComponent>().get<SunComponent>();
    sun.direction = d_cycle.GetSunDir();

    d_postProcessor.AddEffect<GaussianVert>();
    d_postProcessor.AddEffect<GaussianHoriz>();

}

void WorldLayer::LoadScene(std::string_view file)
{
    using namespace Sprocket;

    d_scene.Clear();
    d_scene.Add<ScriptRunner>(d_window);
    d_scene.Add<CameraSystem>(d_window->AspectRatio());
    d_scene.Add<PathFollower>();
    d_scene.Add<GameGrid>(d_window);
    d_scene.Load(file);
    d_paused = false;

    d_sceneFile = file;

    d_worker = d_scene.Entities().Find<NameComponent>([](ecs::Entity entity) {
        return entity.get<NameComponent>().name == "Worker";
    });

    d_camera = d_scene.Entities().Find<NameComponent>([](ecs::Entity entity) {
        return entity.get<NameComponent>().name == "Camera";
    });

    d_scene.Get<GameGrid>().SetCamera(d_camera);

    assert(d_worker != ecs::Null);
    assert(d_camera != ecs::Null);
}

void WorldLayer::OnEvent(Sprocket::ev::Event& event)
{
    using namespace Sprocket;

    // Escape Menu event handling
    if (auto data = event.get_if<ev::KeyboardButtonPressed>()) {
        if (!event.is_consumed() && data->key == Keyboard::ESC) {
            d_paused = !d_paused;
            event.consume();
        }
    }

    if (d_paused) {
        d_escapeMenu.OnEvent(event);
        event.consume();
    }

    // Editor UI event handling
    if (d_mode == Mode::EDITOR) {
        d_devUI.OnEvent(event);
    }

    // Game World event handling
    d_hoveredEntityUI.OnEvent(event);

    if (auto data = event.get_if<ev::WindowResize>()) {
        d_postProcessor.SetScreenSize(data->width, data->height);
    }

    if (auto data = event.get_if<ev::MouseButtonPressed>()) {
        auto& tr = d_camera.get<Transform3DComponent>();
        if (data->mods & KeyModifier::CTRL) {
            glm::vec3 cameraPos = tr.position;
            glm::vec3 direction = Maths::GetMouseRay(
                d_window->GetMousePos(),
                d_window->Width(),
                d_window->Height(),
                MakeView(d_camera),
                MakeProj(d_camera)
            );

            float lambda = -cameraPos.y / direction.y;
            glm::vec3 mousePos = cameraPos + lambda * direction;
            mousePos.y = 0.5f;
            
            auto& path = d_worker.get<PathComponent>();

            if (data->button == Mouse::LEFT) {
                std::queue<glm::vec3>().swap(path.markers);
                auto pos = d_worker.get<Transform3DComponent>().position;
                if (glm::distance(pos, mousePos) > 1.0f) {
                    path.markers = GenerateAStarPath(
                        pos,
                        mousePos,
                        [&](const glm::ivec2& pos) {
                            auto e = d_scene.Get<GameGrid>().At(pos);
                            return e != ecs::Null;
                        }
                    );
                } else {
                    path.markers.push(mousePos);
                }
                event.consume();
            }
            else if (data->button == Mouse::RIGHT) {
                std::queue<glm::vec3>().swap(path.markers);
                event.consume();
            }
        }
    }

    d_scene.OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    using namespace Sprocket;
    Audio::SetListener(d_camera);

    d_hoveredEntityUI.OnUpdate(dt);
    if (!d_paused) {
        d_cycle.OnUpdate(dt);
    }
    
    auto& sun = d_scene.Entities().Find<SunComponent>().get<SunComponent>();
    float factor = (-d_cycle.GetSunDir().y + 1.0f) / 2.0f;
    float facSq = factor * factor;
    auto skyColour = (1.0f - facSq) * NAVY_NIGHT + facSq * LIGHT_BLUE;
    d_window->SetClearColour(skyColour);
    if (d_cycle.IsDay()) {
        sun.direction = d_cycle.GetSunDir();
        sun.colour = {1.0, 0.945, 0.789};
    }
    else {
        sun.direction = -d_cycle.GetSunDir();
        sun.colour = {0.5, 0.57, 0.98};
    }
    sun.brightness = 2.0f * std::abs(glm::cos(d_cycle.GetAngle()));
    sun.direction = glm::normalize(sun.direction);

    if (!d_paused) {
        d_scene.OnUpdate(dt);
    }

    d_devUI.OnUpdate(dt);
    d_escapeMenu.OnUpdate(dt);
}

void WorldLayer::OnRender()
{
    using namespace Sprocket;
    auto& grid = d_scene.Get<GameGrid>();

    // Create the Shadow Map
    float lambda = 5.0f; // TODO: Calculate the floor intersection point
    auto& tc = d_camera.get<Transform3DComponent>();
    glm::vec3 target = tc.position + lambda * Maths::Forwards(tc.orientation);
    d_shadowMap.Draw(
        d_scene.Entities().Find<SunComponent>().get<SunComponent>().direction,
        target,
        d_scene
    );

    if (d_paused) {
        d_postProcessor.Bind();
    }

    d_entityRenderer.EnableShadows(d_shadowMap);
    d_entityRenderer.Draw(d_camera, d_scene);

    if (d_paused) {
        d_postProcessor.Unbind();
        d_postProcessor.Draw();
    }

    if (!d_paused) {
        d_hoveredEntityUI.StartFrame();

        auto mouse = d_window->GetMousePos();
        float w = (float)d_window->Width();
        float h = (float)d_window->Height();

        if (grid.SelectedPosition().has_value()) {
            auto selected = grid.Selected();

            float width = 0.15f * w;
            float height = 0.6f * h;
            float x = w - width;
            float y = ((1.0f - 0.6f) / 2) * h;

            glm::vec4 region{x, y, width, height};
            d_hoveredEntityUI.StartPanel("Selected", &region, PanelType::UNCLICKABLE);
                
            auto pos = grid.SelectedPosition().value();
            if (d_hoveredEntityUI.Button("+Tree", {0, 0, width, 50})) {
                selected.destroy();
                AddTree(pos);
            }

            if (d_hoveredEntityUI.Button("+Rock", {0, 60, width, 50})) {
                selected.destroy();
                AddRock(pos);
            }

            if (d_hoveredEntityUI.Button("+Iron", {0, 120, width, 50})) {
                selected.destroy();
                AddIron(pos);
            }

            if (d_hoveredEntityUI.Button("+Tin", {0, 180, width, 50})) {
                selected.destroy();
                AddTin(pos);
            }

            if (d_hoveredEntityUI.Button("+Mithril", {0, 240, width, 50})) {
                selected.destroy();
                AddMithril(pos);
            }

            if (d_hoveredEntityUI.Button("Clear", {0, 300, width, 50})) {
                selected.destroy();
            }

            d_hoveredEntityUI.EndPanel();
        }


        auto hovered = grid.Hovered();
        if (hovered.valid()) {
            float width = 200;
            float height = 50;
            float x = std::min(mouse.x - 5, w - width - 10);
            float y = std::min(mouse.y - 5, h - height - 10);

            glm::vec4 region{x, y, width, height};
            d_hoveredEntityUI.StartPanel("Hovered", &region, PanelType::UNCLICKABLE);
            std::string name = "Unnamed";
            if (hovered.has<NameComponent>()) {
                name = hovered.get<NameComponent>().name;
            }
            d_hoveredEntityUI.Text(name, 36.0f, {0, 0, width, height});
            d_hoveredEntityUI.EndPanel();
        }

        d_hoveredEntityUI.EndFrame();
    }

    if (d_mode == Mode::EDITOR) {
        d_devUI.StartFrame();

        glm::mat4 view = MakeView(d_camera);
        glm::mat4 proj = MakeProj(d_camera);

        SunInfoPanel(d_devUI, d_cycle);
        ShaderInfoPanel(d_devUI, d_entityRenderer.GetShader());

        ImGui::Begin("Shadow Map");
        ImGuiXtra::Image(d_shadowMap.GetShadowMap(), 500.0f);
        ImGui::End();

        ImGui::ShowDemoWindow();

        d_devUI.EndFrame();
    }

    // Rest of the rendering is the escape menu, which should only get rendered when paused.
    if (!d_paused) {
        return;
    }

    float w = (float)d_window->Width();
    float h = (float)d_window->Height();

    d_escapeMenu.StartFrame();
    static bool showVolume = false;

    glm::vec4 mainRegion{0.0f, 0.0f, w * 0.3f, h};
    d_escapeMenu.StartPanel("Main", &mainRegion, PanelType::CLICKABLE);

    d_escapeMenu.Text("Menu", 48.0f, {0.0f, 0.0f, w * 0.3f, 100});

    glm::vec4 buttonRegion = {w * 0.025f, 100, w * 0.25f, 50};

    if (d_escapeMenu.Button("Toggle Dev UI", buttonRegion)) {
        switch (d_mode) {
            case Mode::PLAYER: {
                d_mode = Mode::EDITOR;
            } break;
            case Mode::EDITOR: {
                d_mode = Mode::PLAYER;
            } break;
        }
    }

    buttonRegion.y += 2 * 60;
    static float value1 = 27.0f;
    d_escapeMenu.Slider("Slider", buttonRegion, &value1, 0, 100);

    buttonRegion.y += 60;
    float angle = d_cycle.GetAngle();
    d_escapeMenu.Dragger("Time of Day", buttonRegion, &angle, 0.001f);
    d_cycle.SetAngle(angle);

    buttonRegion.y += 60;
    d_escapeMenu.Checkbox("Volume Panel", buttonRegion, &showVolume);

    buttonRegion.y += 60;
    if (d_escapeMenu.Button("Reload", buttonRegion)) {
        LoadScene(d_sceneFile);
    }
    
    d_escapeMenu.EndPanel();
    
    static glm::vec4 shape{w/2 - 200, 100, 400, 500};
    if (showVolume) {
        d_escapeMenu.StartPanel("VolumePanel", &shape, PanelType::DRAGGABLE);
        d_escapeMenu.Text("Volume", 48.0f, {0, 0, 400, 100});

        float volume = Sprocket::Audio::GetMasterVolume();
        d_escapeMenu.Slider("Master Volume", {10, 100, 400 - 20, 50}, &volume, 0.0, 100.0);
        Sprocket::Audio::SetMasterVolume(volume);
        
        d_escapeMenu.EndPanel();
    }

    static glm::vec4 shape2{w/2 + 300, 100, 400, 500};
    d_escapeMenu.StartPanel("Button Panel", &shape2, PanelType::DRAGGABLE);
    d_escapeMenu.Text("Buttons", 36.0f, {0, 0, 400, 100});
    glm::vec4 buttonQuad{10, 100, 400 - 20, 50};
    if (d_escapeMenu.Button("Button 1", buttonQuad)) {
        log::warn("Warn");
        log::info("Info");
        log::error("Error");
        log::fatal("Fatal");
    }
    buttonQuad.y += 60;
    d_escapeMenu.Button("Button 2", buttonQuad);
    buttonQuad.y += 60;
    d_escapeMenu.Button("Button 3", buttonQuad);
    buttonQuad.y += 60;
    d_escapeMenu.Button("Button 4", buttonQuad);
    buttonQuad.y += 60;
    d_escapeMenu.Button("Button 5", buttonQuad);
    buttonQuad.y += 60;
    buttonQuad.z = buttonQuad.w;

    static bool valA = false;
    d_escapeMenu.Checkbox("A", buttonQuad, &valA);
    buttonQuad.x += buttonQuad.z + 10.0f;

    static bool valB = true;
    d_escapeMenu.Checkbox("B", buttonQuad, &valB);
    buttonQuad.x += buttonQuad.z + 10.0f;

    d_escapeMenu.EndPanel();
    d_escapeMenu.EndFrame();
}

void WorldLayer::AddTree(const glm::ivec2& pos)
{
    using namespace Sprocket;

    auto newEntity = d_scene.Entities().New();

    auto& name = newEntity.add<NameComponent>();
    name.name = "Tree";

    auto& tr = newEntity.add<Transform3DComponent>();
    tr.orientation = glm::rotate(glm::identity<glm::quat>(), Random(0.0f, 360.0f), {0, 1, 0});
    float r = Random(1.0f, 1.3f);
    tr.scale = {r, r, r};

    auto& modelData = newEntity.add<ModelComponent>();
    modelData.mesh = "Resources/Models/BetterTree.obj";
    modelData.material = "Resources/Materials/tree.yaml";
    newEntity.add<SelectComponent>();

    GridComponent gc = {pos.x, pos.y};
    newEntity.add<GridComponent>(gc);
}

void WorldLayer::AddRockBase(
    const glm::ivec2& pos,
    std::string_view material,
    std::string_view name)
{
    using namespace Sprocket;

    auto newEntity = d_scene.Entities().New();
    auto& n = newEntity.add<NameComponent>();
    n.name = name;

    auto& tr = newEntity.add<Transform3DComponent>();
    tr.position.y -= Random(0.0f, 0.5f);
    float randomRotation = glm::half_pi<float>() * Random(0, 3);
    tr.orientation = glm::rotate(glm::identity<glm::quat>(), randomRotation, {0.0, 1.0, 0.0});
    tr.scale = {1.1f, 1.1f, 1.1f};

    auto& modelData = newEntity.add<ModelComponent>();
    modelData.mesh = "Resources/Models/Rock.obj";
    modelData.material = material;
    newEntity.add<SelectComponent>();

    GridComponent gc = {pos.x, pos.y};
    newEntity.add<GridComponent>(gc);
}

void WorldLayer::AddRock(const glm::ivec2& pos)
{
    AddRockBase(pos, "Resources/Materials/rock.yaml", "Rock");
}

void WorldLayer::AddIron(const glm::ivec2& pos)
{
    AddRockBase(pos, "Resources/Materials/iron.yaml", "Iron");
}

void WorldLayer::AddTin(const glm::ivec2& pos)
{
    AddRockBase(pos, "Resources/Materials/tin.yaml", "Tin");
}

void WorldLayer::AddMithril(const glm::ivec2& pos)
{
    AddRockBase(pos, "Resources/Materials/mithril.yaml", "Mithril");
}