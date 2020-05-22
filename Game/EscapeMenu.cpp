#include "EscapeMenu.h"

void setButtonAttrs(std::shared_ptr<Sprocket::Button> button)
{
    button->base().colour = {0.15625f, 0.15625f, 0.15625f};

    button->buttonNormal().colour = {0.926f, 0.496f, 0.0f};
    button->buttonHovered().colour = {0.926f, 0.63281f, 0.3242f};
    button->buttonClicked().colour = {0.324f, 0.90625f, 0.5352f};
}

void setSliderAttrs(std::shared_ptr<Sprocket::Slider> slider)
{
    slider->base().colour = {0.15625f, 0.15625f, 0.15625f};
    slider->bar().colour = {0.3f, 0.3f, 0.3f};
    slider->picker().colour = {0.926f, 0.496f, 0.0f};

    slider->bar().width = slider->base().width * 0.9f;
    slider->bar().height = slider->base().height * 0.1f;
    slider->bar().position.x = slider->base().width * (1.0f - 0.9f) / 2.0f;
    slider->bar().position.y = slider->base().height * (1.0f - 0.1f) / 2.0f;

    slider->picker().width = slider->base().width * 0.02f;
    slider->picker().height = slider->base().height * 0.8f;
    slider->picker().position.x = slider->base().width * (1.0f - 0.02f) / 2.0f;
    slider->picker().position.y = slider->base().height * (1.0f - 0.8f) / 2.0f;
}

EscapeMenu::EscapeMenu(const Sprocket::CoreSystems& core,
                       WorldLayer* worldLayer,
                       EditorUI* editorUi) 
    : Layer(core, Status::INACTIVE, true)
    , d_worldLayer(worldLayer)
    , d_editorUi(editorUi)
    , d_displayRenderer(core.window)
    , d_container(
        (float)core.window->width()/4.0f,
        {10.0, 10.0},
        10.0f
    )
{
    using namespace Sprocket;

    d_container.position({10.0f, 10.0f});
    d_container.base().colour = {0.07f, 0.07f, 0.07f};
    d_container.base().roundness = 0.081f;
    d_container.addProperty<Draggable>();

    //d_container.addProperty<VerticalConstraint>(VerticalConstraint::Type::TOP, 10.0f);
    //d_container.addProperty<HorizontalConstraint>(HorizontalConstraint::Type::RIGHT, 10.0f);

    auto topSlider = d_container.add<Slider>(300.0f, 50.0f);
    setSliderAttrs(topSlider);

    auto cameraSwitchButton = d_container.add<Button>(
        50.0f, 50.0f, 0.5f, 0.55f, 0.45f
    );

    setButtonAttrs(cameraSwitchButton);

    cameraSwitchButton->setUnclickCallback([&]() {  //unclick
        switch (d_worldLayer->d_mode) {
            case Mode::OBSERVER: {
                SPKT_LOG_INFO("Mode switched to Player");
                d_worldLayer->d_mode = Mode::PLAYER;

                d_worldLayer->d_camera = &d_worldLayer->d_playerCamera;
                d_worldLayer->d_playerMovement.enable(true);
                d_worldLayer->d_mouseRequired = false;
                d_worldLayer->d_selector.enable(false);

                d_editorUi->d_status = Sprocket::Layer::Status::INACTIVE;

            } break;
            case Mode::PLAYER: {
                SPKT_LOG_INFO("Mode switched to Editor");
                d_worldLayer->d_mode = Mode::EDITOR;

                d_worldLayer->d_camera = &d_worldLayer->d_editorCamera;
                d_worldLayer->d_playerMovement.enable(false);
                d_worldLayer->d_mouseRequired = true;
                d_worldLayer->d_selector.enable(true);

                d_editorUi->d_status = Sprocket::Layer::Status::NORMAL;

            } break;
             case Mode::EDITOR: {
                SPKT_LOG_INFO("Mode switched to Observer");
                d_worldLayer->d_mode = Mode::OBSERVER;

                d_worldLayer->d_camera = &d_worldLayer->d_observerCamera;
                d_worldLayer->d_playerMovement.enable(false);
                d_worldLayer->d_mouseRequired = false;
                d_worldLayer->d_selector.enable(false);

                d_editorUi->d_status = Sprocket::Layer::Status::INACTIVE;

            } break;
        }
    });

    auto textBox = d_container.add<TextBox>(
        300.0f, 50.0f, "Text box!"
    );
    textBox->text().position = {10.0, 10.0};
    d_text = textBox.get();
    
    textBox->base().colour = {0.15625f, 0.15625f, 0.15625f};
    textBox->text().font = Sprocket::Font::CALIBRI;

    topSlider->setCallback([textBox](float val) {
        textBox->text().size = 24.0f + val * 60.0f;
    });

    topSlider->setValue(0.0f);

    auto palette = d_container.add<ColourPalette>(300.0f, 300.0f);
    palette->base().colour = {0.15625f, 0.15625f, 0.15625f};
    palette->setCallback([&](const Maths::vec3& colour) {
        for (auto& light : d_worldLayer->d_lights) {
            light.colour = colour;
        }
    });
}

bool EscapeMenu::handleEventImpl(Sprocket::Event& event)
{
    if (auto e = event.as<Sprocket::KeyboardButtonPressedEvent>()) {
        if (e->key() == Sprocket::Keyboard::ESC) {
            d_worldLayer->d_paused = !d_worldLayer->d_paused;
            return true;
        }
    }

    if (d_status == Status::NORMAL) {
        if (d_container.handleEvent(d_core.window, event)) {
            return true;
        }
    }

    return false;
}

void EscapeMenu::updateImpl()
{
    d_status = d_worldLayer->d_paused ? Status::NORMAL : Status::INACTIVE;
    d_displayRenderer.update();

    if (d_status == Status::NORMAL) {
        d_container.update(d_core.window);
        d_core.window->setCursorVisibility(true);
    }
}

void EscapeMenu::drawImpl()
{
    d_container.draw(&d_displayRenderer);
}