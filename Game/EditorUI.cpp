#include "EditorUI.h"
#include "MouseEvent.h"
#include "Camera.h"

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
                        Entity& entity,
                        const Maths::mat4& view,
                        const Maths::mat4& proj)
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
        Maths::vec3 eulerAngles = Maths::ToEuler(tr.orientation);
        std::stringstream ss;
        ss << "Pitch: " << Maths::ToString(eulerAngles.x, 3) << "\n"
           << "Yaw: " << Maths::ToString(eulerAngles.y, 3) << "\n"
           << "Roll: " << Maths::ToString(eulerAngles.z, 3);
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
        Maths::mat4 origin = Maths::Transform(tr.position, tr.orientation);
        ImGuiXtra::Guizmo(&origin, view, proj, mode, coords);
        tr.position = GetTranslation(origin);
        tr.orientation = Normalise(ToQuat(mat3(origin)));
    }
    ImGui::Separator();

    if (ImGui::Button("Delete Entity")) {
        entity.Kill();
    }

    ImGui::End();
}

void SunInfoPanel(DevUI& ui,
                  Sun& sun,
                  CircadianCycle& cycle)
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
        ImGui::DragFloat("Angle", &angle);
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

EditorUI::EditorUI(const CoreSystems& core, WorldLayer* worldLayer)
    : d_core(core)
    , d_worldLayer(worldLayer)
    , d_ui(core.window)
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

    mat4 view = MakeView(d_worldLayer->d_camera);
    mat4 proj = MakeProj(d_worldLayer->d_camera);

    auto e = d_worldLayer->d_selector->SelectedEntity();
    if (!e.Null()) {
        SelectedEntityInfo(d_ui, e, view, proj);
    }

    SunInfoPanel(d_ui, d_worldLayer->d_scene->GetSun(), d_worldLayer->d_cycle);
    ShaderInfoPanel(d_ui, d_worldLayer->d_entityRenderer.GetShader());

    ImGui::Begin("Shadow Map");
    //ImGuiXtra::Image(d_worldLayer->d_shadowMap.GetShadowMap(), 500.0f);
    ImGui::End();

    ImGui::ShowDemoWindow();

    d_ui.EndFrame();
}