#include "EditorUI.h"
#include "MouseEvent.h"

#include <sstream>

namespace {

void AddEntityToList(Sprocket::DevUI::Context& ui,
                     Sprocket::Selector& selector,
                     Sprocket::Entity* entity)
{
    using namespace Sprocket;
    
    //ui.PushID(entity->Id());
    //if (ui.StartTreeNode(entity->Name())) {
    //    if (ui.Button("Select")) {
    //        SPKT_LOG_INFO("Select clicked!");
    //        selector.SetSelected(entity);
    //    }
    //    ui.EndTreeNode();
    //}
    //ui.PopID();         
}

void PhysicsComponentInfo(Sprocket::DevUI::Context& ui,
                          Sprocket::RigidBody3DComponent& phys)
{
    if (ui.StartTreeNode("Physics")) {
        ui.Checkbox("Gravity", &phys.gravity);
        ui.SameLine();
        ui.Checkbox("Frozen", &phys.frozen);
        ui.DragFloat3("Velocity", &phys.velocity, 0.005f);
        ui.SliderFloat("Bounciness", &phys.bounciness, 0.0f, 1.0f);
        ui.SliderFloat("Friction Coeff", &phys.frictionCoefficient, 0.0f, 1.0f);
        ui.SliderFloat("Roll Resistance", &phys.rollingResistance, 0.0f, 1.0f);
        ui.EndTreeNode();
    }
}

void SelectedEntityInfo(Sprocket::DevUI::Context& ui,
                        Sprocket::Entity& entity,
                        const Sprocket::Maths::mat4& view,
                        const Sprocket::Maths::mat4& proj)
{
    using namespace Sprocket;
    using namespace Maths;

    ui.StartWindow("Selected Entity");
    ui.Text("Name: ");
    ui.SameLine();
    //ui.TextModifiable(entity.Name());
    //ui.Text("ID: " + std::to_string(entity.Id()));
    ui.Separator();
    
    static DevUI::GizmoMode mode = DevUI::GizmoMode::TRANSLATION;
    static DevUI::GizmoCoords coords = DevUI::GizmoCoords::WORLD;
    static float angle = 1.0f;

    if (entity.Has<TransformComponent>() && ui.StartTreeNode("Transform")) {
        auto& tr = entity.Get<TransformComponent>();
        ui.DragFloat3("Position", &tr.position, 0.005f);
        Maths::vec3 eulerAngles = Maths::ToEuler(tr.orientation);
        std::stringstream ss;
        ss << "Pitch: " << Maths::ToString(eulerAngles.x, 3) << "\n"
           << "Yaw: " << Maths::ToString(eulerAngles.y, 3) << "\n"
           << "Roll: " << Maths::ToString(eulerAngles.z, 3);
        ui.Text(ss.str());    

        if (ui.RadioButton("Translate", mode == DevUI::GizmoMode::TRANSLATION)) {
            mode = DevUI::GizmoMode::TRANSLATION;
        }
        ui.SameLine();
        if (ui.RadioButton("Rotate", mode == DevUI::GizmoMode::ROTATION)) {
            mode = DevUI::GizmoMode::ROTATION;
        }

        if (ui.RadioButton("World", coords == DevUI::GizmoCoords::WORLD)) {
            coords = DevUI::GizmoCoords::WORLD;
        }
        ui.SameLine();
        if (ui.RadioButton("Local", coords == DevUI::GizmoCoords::LOCAL)) {
            coords = DevUI::GizmoCoords::LOCAL;
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

    if (entity.Has<RigidBody3DComponent>()) {
        PhysicsComponentInfo(ui, entity.Get<RigidBody3DComponent>());
    }

    ui.Separator();

    if (ui.Button("Delete Entity")) {
        entity.Kill();
    }

    if (ui.CollapsingHeader("Add Component")) {
        if (!entity.Has<RigidBody3DComponent>() && ui.Button("Physics")) {

        }
    }

    ui.EndWindow();
}

void AddEntityPanel(Sprocket::DevUI::Context& ui,
                    Sprocket::Scene* entities,
                    Sprocket::ModelManager* models)
{
    ui.StartWindow("Add Entity");
    for (const auto& [name, model] : *models) {
        if (ui.Button(name.c_str())) {
            SPKT_LOG_INFO("Added entity");
            auto entity = entities->NewEntity();
            auto& tr = entity.Add<Sprocket::TransformComponent>();
            tr.position = {10.0, 0.0, 10.0};
            auto& modelComp = entity.Add<Sprocket::ModelComponent>();
            modelComp.model = name;

            entity.Add<Sprocket::SelectComponent>();
        }
    }
    ui.EndWindow();
}

void SunInfoPanel(Sprocket::DevUI::Context& ui,
                  Sprocket::DirectionalLight& sun,
                  float& sunAngle)
{
    ui.StartWindow("Sun");
    ui.ColourPicker("Colour", &sun.colour);
    ui.SliderFloat("Brightness", &sun.brightness, 0.0f, 1.0f);
    ui.DragFloat("Sun Angle", &sunAngle);
    ui.EndWindow();
}

}

EditorUI::EditorUI(const Sprocket::CoreSystems& core, WorldLayer* worldLayer)
    : Layer(core)
    , d_worldLayer(worldLayer)
    , d_ui(core.window)
    , d_modelManager(core.modelManager)
{  
}

void EditorUI::OnEvent(Sprocket::Event& event)
{
    if (d_worldLayer->d_mode != Mode::EDITOR) {
        return;
    }

    d_ui.OnEvent(event);

}

void EditorUI::OnUpdate(double dt)
{
    if (d_worldLayer->d_mode != Mode::EDITOR) {
        return;
    }

    using namespace Sprocket;
    using namespace Maths;

    d_ui.OnUpdate(dt);
    d_ui.StartFrame();

    bool open = true;
    int flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

    d_ui.StartWindow("Sprocket Editor", &open, flags);
    if (d_ui.Button("Physics Engine")) {
        auto& physics = d_worldLayer->d_physicsEngine;
        bool isRunning = physics->Running();
        physics->Running(!isRunning);
    }
    d_ui.SameLine();
    d_ui.Text(d_worldLayer->d_physicsEngine->Running() ? "YES" : "NO");

    if (d_ui.Button("Show Colliders")) {
        auto entityRenderer = &d_worldLayer->d_entityRenderer;
        bool wireframe = entityRenderer->ShowColliders();
        entityRenderer->RenderColliders(!wireframe);
    }
    d_ui.SameLine();
    d_ui.Text(d_worldLayer->d_entityRenderer.ShowColliders() ? "YES" : "NO");

    std::stringstream ss;
    ss << "Entities: " << d_worldLayer->d_scene->Size();
    d_ui.Text(ss.str());

    if (d_ui.CollapsingHeader("Entity List")) {
        d_worldLayer->d_scene->Each<SelectComponent>([&](Entity& entity) {
            AddEntityToList(d_ui, *d_worldLayer->d_selector, &entity);      
        });
    }

    d_ui.EndWindow();

    mat4 view = CameraUtils::MakeView(d_worldLayer->d_activeCamera);
    mat4 proj = CameraUtils::MakeProj(d_worldLayer->d_activeCamera);

    auto e = d_worldLayer->d_selector->SelectedEntity();
    if (!e.Null()) {
        SelectedEntityInfo(d_ui, e, view, proj);
    }

    AddEntityPanel(d_ui, d_worldLayer->d_scene.get(), d_modelManager);

    SunInfoPanel(d_ui, d_worldLayer->d_lights.sun, d_worldLayer->d_sunAngle);

    d_ui.DemoWindow();
    d_ui.EndFrame();
}