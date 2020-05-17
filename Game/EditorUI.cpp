#include "EditorUI.h"

#include <sstream>

#include <imgui.h>

namespace {

void addEntityToList(const Sprocket::Entity& entity)
{
    using namespace Sprocket;
    
    ImGui::PushID((int)entity.id());
    if (ImGui::TreeNode(entity.name().c_str())) {
        ImGui::TreePop();
    }
    ImGui::PopID();      
}

void selectedEntityInfo(Sprocket::Entity& entity)
{
    using namespace Sprocket;
    ImGui::Begin("Selected Entity");
        
    std::string name = "Name: " + entity.name();
    ImGui::Text(name.c_str());
    
    std::string id = "ID: " + std::to_string(entity.id());
    ImGui::Text(id.c_str());
    ImGui::Separator();
    
    if (ImGui::TreeNode("Transform")) {
        ImGui::DragFloat3("Position", &entity.position().x, 0.005f);
        ImGui::Text("TODO: Add Orientation");
        ImGui::TreePop();
    }

    if (entity.has<PhysicsComponent>() && ImGui::TreeNode("Physics")) {
        auto& comp = entity.get<PhysicsComponent>();
        ImGui::Checkbox("Stationary", &comp.stationary);
        ImGui::SameLine();
        ImGui::Checkbox("Gravity", &comp.gravity);
        ImGui::DragFloat("Mass", &comp.mass, 0.05f);
        ImGui::DragFloat3("Velocity", &comp.velocity.x, 0.005f);
        ImGui::SliderFloat("Bounciness", &comp.bounciness, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Friction Coeff", &comp.frictionCoefficient, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Roll Resistance", &comp.rollingResistance, 0.0f, 1.0f, "%.3f");
        ImGui::TreePop();
    }

    ImGui::Separator();

    if (ImGui::Button("Delete Entity")) {
        entity.kill();
    }

    ImGui::End();
}

void addEntityPanel(Sprocket::EntityManager* entities,
                    Sprocket::ModelManager* models)
{
    ImGui::Begin("Add Entity");
    for (const auto& [name, model] : *models) {
        if (ImGui::Button(name.c_str())) {
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
    ImGui::End();
}

}

EditorUI::EditorUI(const Sprocket::CoreSystems& core, WorldLayer* worldLayer)
    : Layer(core, Status::INACTIVE, true)
    , d_worldLayer(worldLayer)
    , d_editorUI(core.window)
    , d_editorUIRenderer(core.window)
    , d_modelManager(core.modelManager)
{  
}

bool EditorUI::handleEventImpl(const Sprocket::Event& event)
{
    if (d_status == Sprocket::Layer::Status::NORMAL) {
        if (d_editorUI.handleEvent(event)) {
            return true;
        }
    }

    return false;
}

void EditorUI::updateImpl()
{
    if (d_status == Sprocket::Layer::Status::NORMAL) {
        d_editorUI.update(deltaTime());
    }
}

void EditorUI::drawImpl()
{
    using namespace Sprocket;

    d_editorUIRenderer.startFrame();

    ImGui::Begin("Sprocket Editor");
    if (ImGui::Button("Physics Engine")) {
        auto& physics = d_worldLayer->d_physicsEngine;
        bool isRunning = physics.running();
        physics.running(!isRunning);
    }

    if (ImGui::Button("Show Colliders")) {
        auto entityRenderer = &d_worldLayer->d_entityRenderer;
        bool wireframe = entityRenderer->showColliders();
        entityRenderer->renderColliders(!wireframe);
    }

    std::stringstream ss;
    ss << "Entities: " << d_worldLayer->d_entityManager.entities().size();
    ImGui::Text(ss.str().c_str());

    if (ImGui::CollapsingHeader("Entity List")) {
        for (auto [id, entity] : d_worldLayer->d_entityManager.entities()) {
            addEntityToList(*entity);      
        }
    }

    ImGui::End();

    if (auto e = d_worldLayer->d_selector.selectedEntity()) {
        selectedEntityInfo(*e);
    }

    addEntityPanel(&d_worldLayer->d_entityManager, d_modelManager);

    bool show = true;
    ImGui::ShowDemoWindow(&show);

    d_editorUIRenderer.endFrame();
    d_editorUIRenderer.draw();
}