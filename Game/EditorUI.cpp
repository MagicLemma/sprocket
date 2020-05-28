#include "EditorUI.h"
#include "MouseEvent.h"

#include <sstream>

namespace {

void addEntityToList(Sprocket::DevUI::Context& ui,
                     const Sprocket::Entity& entity)
{
    using namespace Sprocket;
    
    ui.pushID(entity.id());
    if (ui.startTreeNode(entity.name())) {
        ui.endTreeNode();
    }
    ui.popID();      
}

void selectedEntityInfo(Sprocket::DevUI::Context& ui,
                        Sprocket::Entity& entity,
                        const Sprocket::Maths::mat4& view,
                        const Sprocket::Maths::mat4& proj)
{
    using namespace Sprocket;
    using namespace Maths; 

    ui.startWindow("Selected Entity");
    ui.text("Name: " + entity.name());
    ui.text("ID: " + std::to_string(entity.id()));
    ui.separator();
    
    static DevUI::GizmoMode mode = DevUI::GizmoMode::TRANSLATION;
    static DevUI::GizmoCoords coords = DevUI::GizmoCoords::WORLD;
    static float angle = 1.0f;

    if (ui.startTreeNode("Transform")) {
        ui.dragFloat3("Position", &entity.position(), 0.005f);
        Maths::vec3 eulerAngles = Maths::toEuler(entity.orientation());
        std::stringstream ss;
        ss << "Pitch: " << Maths::toString(eulerAngles.x, 3) << "\n"
           << "Yaw: " << Maths::toString(eulerAngles.y, 3) << "\n"
           << "Roll: " << Maths::toString(eulerAngles.z, 3);
        ui.text(ss.str());    

        if (ui.radioButton("Translate", mode == DevUI::GizmoMode::TRANSLATION)) {
            mode = DevUI::GizmoMode::TRANSLATION;
        }
        ui.sameLine();
        if (ui.radioButton("Rotate", mode == DevUI::GizmoMode::ROTATION)) {
            mode = DevUI::GizmoMode::ROTATION;
        }

        if (ui.radioButton("World", coords == DevUI::GizmoCoords::WORLD)) {
            coords = DevUI::GizmoCoords::WORLD;
        }
        ui.sameLine();
        if (ui.radioButton("Local", coords == DevUI::GizmoCoords::LOCAL)) {
            coords = DevUI::GizmoCoords::LOCAL;
        }
        ui.endTreeNode();
    }

    Maths::mat4 origin = entity.transform();
    ui.gizmo(&origin, view, proj, mode, coords);
    entity.position() = getTranslation(origin);
    entity.orientation() = normalise(toQuat(mat3(origin)));

    if (entity.has<PhysicsComponent>() && ui.startTreeNode("Physics")) {
        auto& comp = entity.get<PhysicsComponent>();
        ui.checkbox("Gravity", &comp.gravity);
        ui.sameLine();
        ui.checkbox("Frozen", &comp.frozen);
        ui.dragFloat3("Velocity", &comp.velocity, 0.005f);
        ui.endTreeNode();
    }

    if (entity.has<ColliderComponent>() && ui.startTreeNode("Collider")) {
        auto& comp = entity.get<ColliderComponent>();
        ui.dragFloat("Mass", &comp.mass, 0.05f);
        ui.sliderFloat("Bounciness", &comp.bounciness, 0.0f, 1.0f);
        ui.sliderFloat("Friction Coeff", &comp.frictionCoefficient, 0.0f, 1.0f);
        ui.sliderFloat("Roll Resistance", &comp.rollingResistance, 0.0f, 1.0f);
        ui.endTreeNode();
    }

    ui.separator();

    if (ui.button("Delete Entity")) {
        entity.kill();
    }

    ui.endWindow();
}

void addEntityPanel(Sprocket::DevUI::Context& ui,
                    Sprocket::EntityManager* entities,
                    Sprocket::ModelManager* models)
{
    ui.startWindow("Add Entity");
    for (const auto& [name, model] : *models) {
        if (ui.button(name.c_str())) {
            SPKT_LOG_INFO("Added entity");
            auto entity = std::make_shared<Sprocket::Entity>();
            entity->position() = {10.0, 0.0, 10.0};
            auto modelComp = entity->add<Sprocket::ModelComponent>();
            modelComp->model = model;

            Sprocket::Material m;
            m.texture = Sprocket::Texture::white();
            
            modelComp->material = m;
            modelComp->scale = 1.0f; 
            entities->addEntity(entity);
        }
    }
    ui.endWindow();
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
    d_ui.startFrame();

    bool open = true;
    int flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;

    d_ui.startWindow("Sprocket Editor", &open, flags);
    if (d_ui.button("Physics Engine")) {
        auto& physics = d_worldLayer->d_physicsEngine;
        bool isRunning = physics.running();
        physics.running(!isRunning);
    }
    d_ui.sameLine();
    d_ui.text(d_worldLayer->d_physicsEngine.running() ? "YES" : "NO");

    if (d_ui.button("Show Colliders")) {
        auto entityRenderer = &d_worldLayer->d_entityRenderer;
        bool wireframe = entityRenderer->showColliders();
        entityRenderer->renderColliders(!wireframe);
    }
    d_ui.sameLine();
    d_ui.text(d_worldLayer->d_entityRenderer.showColliders() ? "YES" : "NO");

    std::stringstream ss;
    ss << "Entities: " << d_worldLayer->d_entityManager.entities().size();
    d_ui.text(ss.str());

    if (d_ui.collapsingHeader("Entity List")) {
        for (auto [id, entity] : d_worldLayer->d_entityManager.entities()) {
            addEntityToList(d_ui, *entity);      
        }
    }

    d_ui.endWindow();

    mat4 view = d_worldLayer->d_camera->view();
    mat4 proj = d_worldLayer->d_lens.projection();
    if (auto e = d_worldLayer->d_selector.selectedEntity()) {
        selectedEntityInfo(d_ui, *e, view, proj);
        d_worldLayer->d_physicsEngine.refreshTransform(e);
    }

    addEntityPanel(d_ui, &d_worldLayer->d_entityManager, d_modelManager);

    d_ui.demoWindow();
    d_ui.endFrame();
}