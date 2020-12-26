#include "Game.h"
#include "Palette.h"
#include "PathFollower.h"
#include "PathCalculator.h"
#include "Window.h"

#include <cmath>

using namespace Sprocket;

namespace {

std::string EntityName(ECS::Entity& entity)
{
    if (entity.Has<NameComponent>()) {
        return entity.Get<NameComponent>().name;
    }
    return "Unnamed";
}

void AddEntityToList(DevUI& ui, BasicSelector& selector, ECS::Entity& entity)
{
    ImGui::PushID(entity.Id());
    if (ImGui::TreeNode(EntityName(entity).c_str())) {
        if (entity.Has<SelectComponent>() && ImGui::Button("Select")) {
            SPKT_LOG_INFO("Select clicked!");
            selector.SetSelected(entity);
        }
        ImGui::TreePop();
    }
    ImGui::PopID();         
}

void SelectedEntityInfo(DevUI& ui,
                        ECS::Entity& entity,
                        const glm::mat4& view,
                        const glm::mat4& proj)
{
    using namespace Maths;

    ImGui::Begin("Selected Entity");
    ImGui::Text("Name: ");
    ImGui::SameLine();
    ImGuiXtra::Text(EntityName(entity));
    ImGuiXtra::Text("ID: " + std::to_string(entity.Id()));
    ImGui::Separator();
    
    static ImGuizmo::OPERATION mode = ImGuizmo::OPERATION::TRANSLATE;
    static ImGuizmo::MODE coords = ImGuizmo::MODE::WORLD;

    if (entity.Has<TransformComponent>() && ImGui::TreeNode("Transform")) {
        auto& tr = entity.Get<TransformComponent>();
        ImGui::DragFloat3("Position", &tr.position.x, 0.005f);
        glm::vec3 eulerAngles = glm::eulerAngles(tr.orientation);
        std::stringstream ss;
        ss << "Pitch: " << Printer::PrintFloat(eulerAngles.x, 3) << "\n"
           << "Yaw: " << Printer::PrintFloat(eulerAngles.y, 3) << "\n"
           << "Roll: " << Printer::PrintFloat(eulerAngles.z, 3);
        ImGui::Text(ss.str().c_str());    

        if (ImGui::RadioButton("Translate", mode == ImGuizmo::OPERATION::TRANSLATE)) {
            mode = ImGuizmo::OPERATION::TRANSLATE;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mode == ImGuizmo::OPERATION::ROTATE)) {
            mode = ImGuizmo::OPERATION::ROTATE;
        }

        if (ImGui::RadioButton("World", coords == ImGuizmo::MODE::WORLD)) {
            coords = ImGuizmo::MODE::WORLD;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Local", coords == ImGuizmo::MODE::LOCAL)) {
            coords = ImGuizmo::MODE::LOCAL;
        }
        ImGui::TreePop();
    }

    if (entity.Has<TransformComponent>()) {
        auto& tr = entity.Get<TransformComponent>();
        glm::mat4 origin = Maths::Transform(tr.position, tr.orientation);
        ImGuiXtra::Guizmo(&origin, view, proj, mode, coords);
        tr.position = GetTranslation(origin);
        tr.orientation = glm::normalize(glm::quat_cast(glm::mat3(origin)));
    }
    ImGui::Separator();

    if (ImGui::Button("Delete Entity")) {
        entity.Delete();
    }

    ImGui::End();
}

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
    , d_scene(std::make_shared<Sprocket::Scene>())
    , d_entityRenderer(&d_assetManager)
    , d_postProcessor(d_window->Width(), d_window->Height())
    , d_gameGrid(std::make_shared<Sprocket::GameGrid>(d_window))
    , d_cameraSystem(std::make_shared<Sprocket::CameraSystem>(d_window->AspectRatio()))
    , d_scriptRunner(std::make_shared<Sprocket::ScriptRunner>(d_window))
    , d_pathFollower(std::make_shared<Sprocket::PathFollower>())
    , d_selector(std::make_shared<Sprocket::BasicSelector>())
    , d_shadowMap(d_window, &d_assetManager)
    , d_hoveredEntityUI(d_window)
    , d_devUI(window)
    , d_escapeMenu(window)
{
    using namespace Sprocket;

    d_scene->AddSystem(d_selector);
    d_scene->AddSystem(d_pathFollower);
    d_scene->AddSystem(d_gameGrid);
    d_scene->AddSystem(d_scriptRunner);
    d_scene->AddSystem(d_cameraSystem);
    LoadScene("Resources/Scene.yaml");

    SimpleUITheme theme;
    theme.backgroundColour = SPACE_DARK;
    theme.baseColour = CLEAR_BLUE;
    theme.hoveredColour = LIGHT_BLUE;
    theme.clickedColour = GARDEN;
    d_hoveredEntityUI.SetTheme(theme);
    d_escapeMenu.SetTheme(theme);    

    d_cycle.SetAngle(3.14195f);

    auto& sun = d_scene->Reg()->Find<SunComponent>().Get<SunComponent>();
    sun.direction = d_cycle.GetSunDir();
    sun.colour = {1.0, 0.945, 0.789};
    sun.brightness = 0.8f;

    auto& ambience = d_scene->GetAmbience();
    ambience.colour = SARAWAK;
    ambience.brightness = 0.01f;

    d_postProcessor.AddEffect<GaussianVert>();
    d_postProcessor.AddEffect<GaussianHoriz>();

    d_scene->OnStartup();
}

void WorldLayer::SaveScene()
{
    SPKT_LOG_INFO("Saving...");
    Sprocket::Loader::Save(d_sceneFile, d_scene);
    SPKT_LOG_INFO("  Done!");
}

void WorldLayer::LoadScene(const std::string& sceneFile)
{
    using namespace Sprocket;

    d_selector->SetSelected(ECS::Null);
    d_paused = false;

    d_sceneFile = sceneFile;
    Loader::Load(sceneFile, d_scene);

    d_worker = d_scene->Reg()->Find<NameComponent>([](ECS::Entity entity) {
        return entity.Get<NameComponent>().name == "Worker";
    });

    d_camera = d_scene->Reg()->Find<NameComponent>([](ECS::Entity entity) {
        return entity.Get<NameComponent>().name == "Camera";
    });

    d_gameGrid->SetCamera(d_camera);

    assert(d_worker != ECS::Null);
    assert(d_camera != ECS::Null);
}

void WorldLayer::OnEvent(Sprocket::Event& event)
{
    using namespace Sprocket;

    // Escape Menu event handling
    if (auto e = event.As<Sprocket::KeyboardButtonPressedEvent>()) {
        if (!e->IsConsumed() && e->Key() == Sprocket::Keyboard::ESC) {
            d_paused = !d_paused;
            e->Consume();
        }
    }

    if (d_paused) {
        d_escapeMenu.OnEvent(event);
        event.Consume();
    }

    // Editor UI event handling
    if (d_mode == Mode::EDITOR) {
        d_devUI.OnEvent(event);

        if (!event.IsConsumed()) {
            if (auto e = event.As<MouseButtonPressedEvent>()) {
                d_selector->SetSelected(ECS::Null);
                // TODO: Do we want to consume this event here?
            }
        }
    }

    // Game World event handling
    d_hoveredEntityUI.OnEvent(event);

    if (auto e = event.As<WindowResizeEvent>()) {
        d_postProcessor.SetScreenSize(e->Width(), e->Height());
    }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        auto& tr = d_camera.Get<TransformComponent>();
        if (e->Mods() & KeyModifier::CTRL) {
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
            
            auto& path = d_worker.Get<PathComponent>();

            if (e->Button() == Mouse::LEFT) {
                std::queue<glm::vec3>().swap(path.markers);
                auto pos = d_worker.Get<TransformComponent>().position;
                if (glm::distance(pos, mousePos) > 1.0f) {
                    path.markers = GenerateAStarPath(
                        pos,
                        mousePos,
                        [&](const glm::ivec2& pos) {
                            auto e = d_gameGrid->At(pos.x, pos.y);
                            return e != ECS::Null;
                        }
                    );
                } else {
                    path.markers.push(mousePos);
                }
                e->Consume();
            }
            else if (e->Button() == Mouse::RIGHT) {
                std::queue<glm::vec3>().swap(path.markers);
                e->Consume();
            }
        }
    }

    d_scene->OnEvent(event);
}

void WorldLayer::OnUpdate(double dt)
{
    using namespace Sprocket;
    Audio::SetListener(d_camera);

    d_hoveredEntityUI.OnUpdate(dt);
    if (!d_paused) {
        d_cycle.OnUpdate(dt);
    }
    
    auto& sun = d_scene->Reg()->Find<SunComponent>().Get<SunComponent>();
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
        d_scene->OnUpdate(dt);
    }

    d_devUI.OnUpdate(dt);
    d_escapeMenu.OnUpdate(dt);
}

void WorldLayer::OnRender()
{
    using namespace Sprocket;

    // Create the Shadow Map
    float lambda = 5.0f; // TODO: Calculate the floor intersection point
    glm::vec3 target = d_camera.Get<TransformComponent>().position + lambda * Maths::Forwards(d_camera.Get<TransformComponent>().orientation);
    d_shadowMap.Draw(
        d_scene->Reg()->Find<SunComponent>().Get<SunComponent>().direction,
        target,
        *d_scene
    );

    if (d_paused) {
        d_postProcessor.Bind();
    }

    d_entityRenderer.EnableShadows(d_shadowMap);
    d_entityRenderer.Draw(d_camera, *d_scene);

    if (d_paused) {
        d_postProcessor.Unbind();
        d_postProcessor.Draw();
    }

    if (!d_paused) {
        d_hoveredEntityUI.StartFrame();

        auto mouse = d_window->GetMousePos();
        float w = (float)d_window->Width();
        float h = (float)d_window->Height();

        if (d_gameGrid->SelectedPosition().has_value()) {
            auto selected = d_gameGrid->Selected();

            float width = 0.15f * w;
            float height = 0.6f * h;
            float x = w - width;
            float y = ((1.0f - 0.6f) / 2) * h;

            glm::vec4 region{x, y, width, height};
            d_hoveredEntityUI.StartPanel("Selected", &region, PanelType::UNCLICKABLE);
                
            auto pos = d_gameGrid->SelectedPosition().value();
            if (d_hoveredEntityUI.Button("+Tree", {0, 0, width, 50})) {
                selected.Delete();
                AddTree(pos);
            }

            if (d_hoveredEntityUI.Button("+Rock", {0, 60, width, 50})) {
                selected.Delete();
                AddRock(pos);
            }

            if (d_hoveredEntityUI.Button("+Iron", {0, 120, width, 50})) {
                selected.Delete();
                AddIron(pos);
            }

            if (d_hoveredEntityUI.Button("+Tin", {0, 180, width, 50})) {
                selected.Delete();
                AddTin(pos);
            }

            if (d_hoveredEntityUI.Button("+Mithril", {0, 240, width, 50})) {
                selected.Delete();
                AddMithril(pos);
            }

            if (d_hoveredEntityUI.Button("Clear", {0, 300, width, 50})) {
                selected.Delete();
            }

            d_hoveredEntityUI.EndPanel();
        }


        auto hovered = d_gameGrid->Hovered();
        if (hovered.Valid()) {
            float width = 200;
            float height = 50;
            float x = std::min(mouse.x - 5, w - width - 10);
            float y = std::min(mouse.y - 5, h - height - 10);

            glm::vec4 region{x, y, width, height};
            d_hoveredEntityUI.StartPanel("Hovered", &region, PanelType::UNCLICKABLE);
            std::string name = "Unnamed";
            if (hovered.Has<NameComponent>()) {
                name = hovered.Get<NameComponent>().name;
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

        auto e = d_selector->SelectedEntity();
        if (e.Valid()) {
            SelectedEntityInfo(d_devUI, e, view, proj);
        }

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
    if (d_escapeMenu.Button("Save", buttonRegion)) {
        SaveScene();
    }

    buttonRegion.y += 60;
    if (d_escapeMenu.Button("Open", buttonRegion)) {
        std::string newScene = OpenFile(d_window, "");
        LoadScene(newScene);
    }

    buttonRegion.y += 60;
    d_escapeMenu.Checkbox("Volume Panel", buttonRegion, &showVolume);
    
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
    d_escapeMenu.Button("Button 1", buttonQuad);
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

    auto newEntity = d_scene->Reg()->New();

    auto& name = newEntity.Add<NameComponent>();
    name.name = "Tree";

    auto& tr = newEntity.Add<TransformComponent>();
    tr.orientation = glm::rotate(glm::identity<glm::quat>(), Random(0.0f, 360.0f), {0, 1, 0});
    float r = Random(1.0f, 1.3f);
    tr.scale = {r, r, r};

    auto& modelData = newEntity.Add<ModelComponent>();
    modelData.mesh = "Resources/Models/BetterTree.obj";
    modelData.material = "Resources/Materials/tree.yaml";
    newEntity.Add<SelectComponent>();

    GridComponent gc = {pos.x, pos.y};
    newEntity.Add<GridComponent>(gc);
}

void WorldLayer::AddRockBase(
    const glm::ivec2& pos,
    const std::string& material,
    const std::string& name)
{
    using namespace Sprocket;

    auto newEntity = d_scene->Reg()->New();
    auto& n = newEntity.Add<NameComponent>();
    n.name = name;

    auto& tr = newEntity.Add<TransformComponent>();
    tr.position.y -= Random(0.0f, 0.5f);
    float randomRotation = glm::half_pi<float>() * Random(0, 3);
    tr.orientation = glm::rotate(glm::identity<glm::quat>(), randomRotation, {0.0, 1.0, 0.0});
    tr.scale = {1.1f, 1.1f, 1.1f};

    auto& modelData = newEntity.Add<ModelComponent>();
    modelData.mesh = "Resources/Models/Rock.obj";
    modelData.material = material;
    newEntity.Add<SelectComponent>();

    GridComponent gc = {pos.x, pos.y};
    newEntity.Add<GridComponent>(gc);
}

void WorldLayer::AddRock(const glm::ivec2& pos)
{
    static std::string material = "Resources/Materials/rock.yaml";
    AddRockBase(pos, material, "Rock");
}

void WorldLayer::AddIron(const glm::ivec2& pos)
{
    static std::string material = "Resources/Materials/iron.yaml";
    AddRockBase(pos, material, "Iron");
}

void WorldLayer::AddTin(const glm::ivec2& pos)
{
    static std::string material = "Resources/Materials/tin.yaml";
    AddRockBase(pos, material, "Tin");
}

void WorldLayer::AddMithril(const glm::ivec2& pos)
{
    static std::string material = "Resources/Materials/mithril.yaml";
    AddRockBase(pos, material, "Mithril");
}