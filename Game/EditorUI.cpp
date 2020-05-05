#include "EditorUI.h"

#include <sstream>

namespace {

void setButtonAttrs(std::shared_ptr<Sprocket::Button> button)
{
    button->base().colour = {0.15625f, 0.15625f, 0.15625f};

    button->buttonNormal().colour = {0.926f, 0.496f, 0.0f};
    button->buttonHovered().colour = {0.926f, 0.63281f, 0.3242f};
    button->buttonClicked().colour = {0.324f, 0.90625f, 0.5352f};
}

}

EditorUI::EditorUI(Sprocket::Accessor& accessor, WorldLayer* worldLayer)
    : Layer(accessor, Status::INACTIVE, true)
    , d_container(
        200.0f,
        {10.0, 10.0},
        10.0f
    )
    , d_entityAttrs(
        400.0f,
        {10.0, 10.0},
        10.0f
    )
    , d_worldLayer(worldLayer)
    , d_displayRenderer(accessor.window())
{
    using namespace Sprocket;

    d_container.position({10.0f, 10.0f});
    d_container.base().colour = {0.07f, 0.07f, 0.07f};
    d_container.base().roundness = 0.2f;
    d_container.addProperty<HorizontalConstraint>(HorizontalConstraint::Type::RIGHT, 10.0f);

    auto button = d_container.add<Button>(50.0f, 50.0f);
    setButtonAttrs(button);
    button->setUnclickCallback([&]() {
        auto& physics = d_worldLayer->d_physicsEngine;
        bool isRunning = physics.running();
        physics.running(!isRunning);
    });

    auto showCollidersBtn = d_container.add<Button>(
        50.0f, 50.0f, 0.5f, 0.55f, 0.45f
    );
    setButtonAttrs(showCollidersBtn);
    showCollidersBtn->buttonNormal().colour = {0.426f, 0.296f, 1.0f};
    showCollidersBtn->setUnclickCallback([&]() {
        static bool val = false;
        val = !val;
        d_worldLayer->d_entityRenderer.renderColliders(val);
    });
    
    d_entityAttrs.position({10.0f, 10.0f});
    d_entityAttrs.base().colour = {0.07f, 0.07f, 0.07f};
    d_entityAttrs.base().roundness = 0.2f;
    d_entityAttrs.addProperty<HorizontalConstraint>(HorizontalConstraint::Type::RIGHT, 10.0f);
    d_entityAttrs.addProperty<VerticalConstraint>(VerticalConstraint::Type::BOTTOM, 10.0f);

    auto positionText = d_entityAttrs.add<TextBox>(d_entityAttrs.width() - 20.0f, 20.0f, "");
    positionText->base().opacity = 0; 
    positionText->text().colour = {0.85625f, 0.85625f, 0.85625f};
    d_positionText = positionText.get();

    auto velocityText = d_entityAttrs.add<TextBox>(d_entityAttrs.width() - 20.0f, 20.0f, "");
    velocityText->base().opacity = 0;
    velocityText->text().colour = {0.85625f, 0.85625f, 0.85625f};
    d_velocityText = velocityText.get();

    auto speedText = d_entityAttrs.add<TextBox>(d_entityAttrs.width() - 20.0f, 20.0f, "");
    speedText->base().opacity = 0;
    speedText->text().colour = {0.85625f, 0.85625f, 0.85625f};
    d_speedText = speedText.get();

    auto deleteButton = d_entityAttrs.add<Button>(50.0f, 50.0f);
    deleteButton->base().colour = {0.15625f, 0.15625f, 0.15625f};
    deleteButton->buttonNormal().colour = {0.8, 0.0, 0.0};
    deleteButton->buttonHovered().colour = {0.4, 0.0, 0.0};
    deleteButton->buttonClicked().colour = {1.0, 0.0, 0.0};
    deleteButton->setUnclickCallback([&]() {
        if (auto e = d_worldLayer->d_selector.selectedEntity()) {
            d_worldLayer->d_entityManager.deleteEntity(e);
        }
    });
    d_deleteButton = deleteButton.get();
}

bool EditorUI::handleEventImpl(const Sprocket::Event& event)
{
    if (d_status == Sprocket::Layer::Status::NORMAL) {
        if (d_container.handleEvent(d_accessor.window(), event)) {
            return true;
        }
        if (d_entityAttrs.handleEvent(d_accessor.window(), event)) {
            return true;
        }
    }

    return false;
}

void EditorUI::updateImpl()
{
    using namespace Sprocket;

    // Don't do anything on this layer if it is not NORMAL,
    // this status of this layer is changed by the EscapeMenu layer.
    if (d_status == Layer::Status::NORMAL) {
        d_displayRenderer.update();
        d_container.update(d_accessor.window());
        d_entityAttrs.update(d_accessor.window()); 

        if (auto e = d_worldLayer->d_selector.selectedEntity()) {
            d_entityAttrs.active(true);

            Maths::vec3 pos = e->get<TransformComponent>().position;
            d_positionText->text().message = "Position: " + Maths::to_string(pos, 3);
        
            if (!e->has<PhysicsComponent>()) {
                d_velocityText->text().message = "N/A";
                d_speedText->text().message = "N/A";
            }
            else {
                Maths::vec3 vel = e->get<PhysicsComponent>().velocity;
                std::stringstream ss;
                ss << "Speed: " << Maths::magnitude(vel);
                d_velocityText->text().message = "Velocity: " + Maths::to_string(vel, 3);
                d_speedText->text().message = ss.str();
            }
        }
        else {
            d_entityAttrs.active(false);
            d_positionText->text().message = "Position: No Entity Selected";
            d_velocityText->text().message = "Velocity: No Entity Selected";
            d_speedText->text().message = "Speed: No Entity Selected";
        }

        
    }
}

void EditorUI::drawImpl()
{
    d_container.draw(&d_displayRenderer);
    d_entityAttrs.draw(&d_displayRenderer);
}