#include "EditorUI.h"
#include "MouseEvent.h"

#include <sstream>

namespace {

void AddEntityToList(Sprocket::DevUI::Context& ui,
                     Sprocket::BasicSelector& selector,
                     Sprocket::Entity* entity)
{
    using namespace Sprocket;
    
    ui.PushID(entity->Id());
    if (ui.StartTreeNode(entity->Name())) {
        if (entity->Has<SelectComponent>() && ui.Button("Select")) {
            SPKT_LOG_INFO("Select clicked!");
            selector.SetSelected(entity);
        }
        ui.EndTreeNode();
    }
    ui.PopID();         
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
    ui.TextModifiable(entity.Name());
    ui.Text("ID: " + std::to_string(entity.Id()));
    ui.Separator();
    
    static DevUI::GizmoMode mode = DevUI::GizmoMode::TRANSLATION;
    static DevUI::GizmoCoords coords = DevUI::GizmoCoords::WORLD;

    if (ui.StartTreeNode("Transform")) {
        ui.DragFloat3("Position", &entity.Position(), 0.005f);
        Maths::vec3 eulerAngles = Maths::ToEuler(entity.Orientation());
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

    Maths::mat4 origin = entity.Transform();
    ui.Gizmo(&origin, view, proj, mode, coords);
    entity.Position() = GetTranslation(origin);
    entity.Orientation() = Normalise(ToQuat(mat3(origin)));
    ui.Separator();

    if (ui.Button("Delete Entity")) {
        entity.Kill();
    }

    ui.EndWindow();
}

void AddEntityPanel(Sprocket::DevUI::Context& ui,
                    Sprocket::EntityManager* entities,
                    Sprocket::ModelManager* models)
{
    ui.StartWindow("Add Entity");
    for (const auto& [name, model] : *models) {
        if (ui.Button(name.c_str())) {
            SPKT_LOG_INFO("Added entity");
            auto entity = std::make_shared<Sprocket::Entity>();
            entity->Position() = {10.0, 0.0, 10.0};
            auto modelComp = entity->Add<Sprocket::ModelComponent>();
            modelComp->model = model;

            Sprocket::Material m;
            m.texture = Sprocket::Texture::White();

            entity->Add<Sprocket::SelectComponent>();
            
            modelComp->material = m;
            modelComp->scale = 1.0f; 
            entities->AddEntity(entity);
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
    ui.SliderFloat("Brightness", &sun.brightness, 0.0f, 30.0f);
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
    using namespace Sprocket;

    if (d_worldLayer->d_mode != Mode::EDITOR) {
        return;
    }

    d_ui.OnEvent(event);

    if (!event.IsConsumed()) {
        if (auto e = event.As<MouseButtonPressedEvent>()) {
            d_worldLayer->d_selector.SetSelected(nullptr);
            // TODO: Do we want to consume this event here?
        }
    }

}

void EditorUI::OnUpdate(float dt)
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
    std::stringstream ss;
    ss << "Entities: " << d_worldLayer->d_entityManager.Entities().size();
    d_ui.Text(ss.str());

    if (d_ui.CollapsingHeader("Entity List")) {
        for (auto [id, entity] : d_worldLayer->d_entityManager.Entities()) {
            AddEntityToList(d_ui, d_worldLayer->d_selector, entity.get());      
        }
    }

    d_ui.EndWindow();

    mat4 view = d_worldLayer->d_camera.View();
    mat4 proj = d_worldLayer->d_lens.Projection();
    if (auto e = d_worldLayer->d_selector.SelectedEntity()) {
        SelectedEntityInfo(d_ui, *e, view, proj);
    }

    AddEntityPanel(d_ui, &d_worldLayer->d_entityManager, d_modelManager);
    SunInfoPanel(d_ui, d_worldLayer->d_lights.sun, d_worldLayer->d_sunAngle);

    d_ui.StartWindow("Shadow Map", &open);

    ImTextureID id = (void*)(intptr_t)d_worldLayer->d_shadowMap.DepthTexId();
    ImGui::Image(id, ImVec2(d_worldLayer->d_shadowMap.Width(), d_worldLayer->d_shadowMap.Height()), ImVec2(0.0, 1.0), ImVec2(1.0, 0.0), ImVec4(1.0, 1.0, 1.0, 1.0), ImVec4(1.0, 1.0, 1.0, 0.5));

    d_ui.EndWindow();

    d_ui.DemoWindow();
    d_ui.EndFrame();
}