#include "EditorUI.h"
#include "MouseEvent.h"

#include <sstream>

namespace {

void addEntityToList(Sprocket::DevUI::Context& ui,
                     Sprocket::Selector& selector,
                     Sprocket::Entity* entity)
{
    using namespace Sprocket;
    
    ui.PushID(entity->id());
    if (ui.StartTreeNode(entity->name())) {
        if (ui.Button("Select")) {
            SPKT_LOG_INFO("Select clicked!");
            selector.setSelected(entity);
        }
        ui.EndTreeNode();
    }
    ui.PopID();         
}

void selectedEntityInfo(Sprocket::DevUI::Context& ui,
                        Sprocket::Entity& entity,
                        const Sprocket::Maths::mat4& view,
                        const Sprocket::Maths::mat4& proj)
{
    using namespace Sprocket;
    using namespace Maths;

    ui.StartWindow("Selected Entity");
    ui.Text("Name: ");
    ui.SameLine();
    ui.TextModifiable(entity.name());
    ui.Text("ID: " + std::to_string(entity.id()));
    ui.Separator();
    
    static DevUI::GizmoMode mode = DevUI::GizmoMode::TRANSLATION;
    static DevUI::GizmoCoords coords = DevUI::GizmoCoords::WORLD;
    static float angle = 1.0f;

    if (ui.StartTreeNode("Transform")) {
        ui.DragFloat3("Position", &entity.position(), 0.005f);
        Maths::vec3 eulerAngles = Maths::toEuler(entity.orientation());
        std::stringstream ss;
        ss << "Pitch: " << Maths::toString(eulerAngles.x, 3) << "\n"
           << "Yaw: " << Maths::toString(eulerAngles.y, 3) << "\n"
           << "Roll: " << Maths::toString(eulerAngles.z, 3);
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

    Maths::mat4 origin = entity.transform();
    ui.Gizmo(&origin, view, proj, mode, coords);
    entity.position() = getTranslation(origin);
    entity.orientation() = normalise(toQuat(mat3(origin)));

    if (entity.has<PhysicsComponent>() && ui.StartTreeNode("Physics")) {
        auto& comp = entity.get<PhysicsComponent>();
        ui.Checkbox("Gravity", &comp.gravity);
        ui.SameLine();
        ui.Checkbox("Frozen", &comp.frozen);
        ui.DragFloat3("Velocity", &comp.velocity, 0.005f);
        ui.EndTreeNode();
    }

    if (entity.has<ColliderComponent>() && ui.StartTreeNode("Collider")) {
        auto& comp = entity.get<ColliderComponent>();
        ui.DragFloat("Mass", &comp.mass, 0.05f);
        ui.SliderFloat("Bounciness", &comp.bounciness, 0.0f, 1.0f);
        ui.SliderFloat("Friction Coeff", &comp.frictionCoefficient, 0.0f, 1.0f);
        ui.SliderFloat("Roll Resistance", &comp.rollingResistance, 0.0f, 1.0f);
        ui.EndTreeNode();
    }

    ui.Separator();

    if (ui.Button("Delete Entity")) {
        entity.kill();
    }

    if (ui.CollapsingHeader("Add Component")) {
        if (!entity.has<PhysicsComponent>() && ui.Button("Physics")) {

        }
        if (!entity.has<ColliderComponent>() && ui.Button("Colldier")) {

        }
    }

    ui.EndWindow();
}

void addEntityPanel(Sprocket::DevUI::Context& ui,
                    Sprocket::EntityManager* entities,
                    Sprocket::ModelManager* models)
{
    ui.StartWindow("Add Entity");
    for (const auto& [name, model] : *models) {
        if (ui.Button(name.c_str())) {
            SPKT_LOG_INFO("Added entity");
            auto entity = std::make_shared<Sprocket::Entity>();
            entity->position() = {10.0, 0.0, 10.0};
            auto modelComp = entity->add<Sprocket::ModelComponent>();
            modelComp->model = model;

            Sprocket::Material m;
            m.texture = Sprocket::Texture::white();

            entity->add<Sprocket::SelectComponent>();
            
            modelComp->material = m;
            modelComp->scale = 1.0f; 
            entities->addEntity(entity);
        }
    }
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

void EditorUI::handleEvent(Sprocket::Event& event)
{
    if (d_worldLayer->d_mode != Mode::EDITOR) {
        return;
    }

    d_ui.handleEvent(event);

}

void EditorUI::update(float dt)
{
    if (d_worldLayer->d_mode != Mode::EDITOR) {
        return;
    }

    using namespace Sprocket;
    using namespace Maths;

    d_ui.update(dt);
    d_ui.StartFrame();

    bool open = true;
    int flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

    d_ui.StartWindow("Sprocket Editor", &open, flags);
    if (d_ui.Button("Physics Engine")) {
        auto& physics = d_worldLayer->d_physicsEngine;
        bool isRunning = physics.running();
        physics.running(!isRunning);
    }
    d_ui.SameLine();
    d_ui.Text(d_worldLayer->d_physicsEngine.running() ? "YES" : "NO");

    if (d_ui.Button("Show Colliders")) {
        auto entityRenderer = &d_worldLayer->d_entityRenderer;
        bool wireframe = entityRenderer->showColliders();
        entityRenderer->renderColliders(!wireframe);
    }
    d_ui.SameLine();
    d_ui.Text(d_worldLayer->d_entityRenderer.showColliders() ? "YES" : "NO");

    std::stringstream ss;
    ss << "Entities: " << d_worldLayer->d_entityManager.entities().size();
    d_ui.Text(ss.str());

    if (d_ui.CollapsingHeader("Entity List")) {
        for (auto [id, entity] : d_worldLayer->d_entityManager.entities()) {
            addEntityToList(d_ui, d_worldLayer->d_selector, entity.get());      
        }
    }

    d_ui.EndWindow();

    mat4 view = d_worldLayer->d_camera->view();
    mat4 proj = d_worldLayer->d_lens.projection();
    if (auto e = d_worldLayer->d_selector.selectedEntity()) {
        selectedEntityInfo(d_ui, *e, view, proj);
        d_worldLayer->d_physicsEngine.refreshTransform(e);
    }

    addEntityPanel(d_ui, &d_worldLayer->d_entityManager, d_modelManager);

    d_ui.DemoWindow();
    d_ui.EndFrame();
}