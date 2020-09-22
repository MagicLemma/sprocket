#include "EditorUI.h"
#include "MouseEvent.h"
#include "CameraUtils.h"

#include <sstream>

using namespace Sprocket;

namespace {

std::string EntityName(Entity& entity)
{
    if (entity.Has<NameComponent>()) {
        return entity.Get<NameComponent>().name;
    }
    return "Unnamed";
}

void AddEntityToList(DevUI& ui, BasicSelector& selector, Entity& entity)
{
    ui.PushID(entity.Id());
    if (ui.StartTreeNode(EntityName(entity))) {
        if (entity.Has<SelectComponent>() && ui.Button("Select")) {
            SPKT_LOG_INFO("Select clicked!");
            selector.SetSelected(entity);
        }
        ui.EndTreeNode();
    }
    ui.PopID();         
}

void SelectedEntityInfo(DevUI& ui,
                        Entity& entity,
                        const Maths::mat4& view,
                        const Maths::mat4& proj)
{
    using namespace Maths;

    ui.StartWindow("Selected Entity");
    ui.Text("Name: ");
    ui.SameLine();
    ui.Text(EntityName(entity));
    ui.Text("ID: " + std::to_string(entity.Id()));
    ui.Separator();
    
    static GizmoMode mode = GizmoMode::TRANSLATION;
    static GizmoCoords coords = GizmoCoords::WORLD;

    if (entity.Has<TransformComponent>() && ui.StartTreeNode("Transform")) {
        auto& tr = entity.Get<TransformComponent>();
        ui.DragFloat3("Position", &tr.position, 0.005f);
        Maths::vec3 eulerAngles = Maths::ToEuler(tr.orientation);
        std::stringstream ss;
        ss << "Pitch: " << Maths::ToString(eulerAngles.x, 3) << "\n"
           << "Yaw: " << Maths::ToString(eulerAngles.y, 3) << "\n"
           << "Roll: " << Maths::ToString(eulerAngles.z, 3);
        ui.Text(ss.str());    

        if (ui.RadioButton("Translate", mode == GizmoMode::TRANSLATION)) {
            mode = GizmoMode::TRANSLATION;
        }
        ui.SameLine();
        if (ui.RadioButton("Rotate", mode == GizmoMode::ROTATION)) {
            mode = GizmoMode::ROTATION;
        }

        if (ui.RadioButton("World", coords == GizmoCoords::WORLD)) {
            coords = GizmoCoords::WORLD;
        }
        ui.SameLine();
        if (ui.RadioButton("Local", coords == GizmoCoords::LOCAL)) {
            coords = GizmoCoords::LOCAL;
        }
        ui.EndTreeNode();
    }

    if (entity.Has<TransformComponent>()) {
        auto& tr = entity.Get<TransformComponent>();
        Maths::mat4 origin = Maths::Transform(tr.position, tr.orientation);
        ui.Gizmo(&origin, view, proj, mode, coords);
        tr.position = GetTranslation(origin);
        tr.orientation = Normalise(ToQuat(mat3(origin)));
    }
    ui.Separator();

    if (ui.Button("Delete Entity")) {
        entity.Kill();
    }

    ui.EndWindow();
}

void SunInfoPanel(DevUI& ui,
                  Sun& sun,
                  CircadianCycle& cycle)
{
    ui.StartWindow("Sun");

    if (ui.Button("Start")) { cycle.Start(); }
    ui.SameLine();
    if (ui.Button("Stop")) { cycle.Stop(); }
    ui.Separator();

    if (cycle.IsRunning()) { // If running, be able to change the speed.
        float speed = cycle.GetSpeed();
        ui.SliderFloat("Speed", &speed, 1.0f, 1000.0f);
        cycle.SetSpeed(speed);
    }
    else { // If not running, be able to set manually
        float angle = cycle.GetAngle();
        ui.DragFloat("Angle", &angle);
        cycle.SetAngle(angle);
    }
    
    ui.Separator();
    ui.Text(cycle.ToString12Hour());
    ui.EndWindow();
}

void ShaderInfoPanel(DevUI& ui, Shader& shader)
{
    static std::string compileStatus;

    ui.StartWindow("Shader");
    if(ui.Button("Recompile")) {
        bool result = shader.Reload();
        compileStatus ="Shader compile:";
        compileStatus += (result? " SUCCESS": " FAILURE");
    }

    ui.Text(compileStatus.c_str());
    
    bool closed = true;
    if(ui.CollapsingHeader("Vertex")){
        closed = false;
        ui.MultilineTextModifiable("", shader.VertexShaderSource());
    }
    if(ui.CollapsingHeader("Frag")) {
        closed = false;
        ui.MultilineTextModifiable("", shader.FragShaderSource());
    }
    
    if(closed) {
        compileStatus.clear();
    }

    ui.EndWindow();
}

}

EditorUI::EditorUI(const CoreSystems& core, WorldLayer* worldLayer)
    : Layer(core)
    , d_worldLayer(worldLayer)
    , d_ui(core.window)
    , d_modelManager(core.modelManager)
{  
}

void EditorUI::OnEvent(Event& event)
{
    if (d_worldLayer->d_mode != Mode::EDITOR) {
        return;
    }

    d_ui.OnEvent(event);

    if (!event.IsConsumed()) {
        if (auto e = event.As<MouseButtonPressedEvent>()) {
            d_worldLayer->d_selector->SetSelected(Entity());
            // TODO: Do we want to consume this event here?
        }
    }

}

void EditorUI::OnUpdate(double dt)
{
    if (d_worldLayer->d_mode != Mode::EDITOR) {
        return;
    }

    using namespace Maths;

    d_ui.OnUpdate(dt);
    d_ui.StartFrame();

    bool open = true;
    int flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

    d_ui.StartWindow("Sprocket Editor", &open, flags);
    std::stringstream ss;
    ss << "Entities: " << d_worldLayer->d_scene->Size();
    d_ui.Text(ss.str());

    if (d_ui.CollapsingHeader("Entity List")) {
        d_worldLayer->d_scene->Each<SelectComponent>([&](Entity& entity) {
            AddEntityToList(d_ui, *d_worldLayer->d_selector, entity);      
        });
    }

    d_ui.EndWindow();

    mat4 view = CameraUtils::MakeView(d_worldLayer->d_camera);
    mat4 proj = CameraUtils::MakeProj(d_worldLayer->d_camera);

    auto e = d_worldLayer->d_selector->SelectedEntity();
    if (!e.Null()) {
        SelectedEntityInfo(d_ui, e, view, proj);
    }

    SunInfoPanel(d_ui, d_worldLayer->d_scene->GetSun(), d_worldLayer->d_cycle);
    ShaderInfoPanel(d_ui, d_worldLayer->d_entityRenderer.GetShader());

    d_ui.StartWindow("Shadow Map", &open);

    auto shadowMap = d_worldLayer->d_shadowMap.GetShadowMap();
    ImTextureID id = (void*)(intptr_t)shadowMap.Id();
    ImGui::Image(id, ImVec2(shadowMap.Width(), shadowMap.Height()), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0), ImVec4(1.0, 1.0, 1.0, 1.0), ImVec4(1.0, 1.0, 1.0, 0.5));

    d_ui.EndWindow();

    d_ui.DemoWindow();
    d_ui.EndFrame();
}