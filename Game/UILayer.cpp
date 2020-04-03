#include "UILayer.h"

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

UILayer::UILayer(Sprocket::Accessor& accessor,
                 std::shared_ptr<BasicSceneInfo> info) 
    : Layer(accessor, Status::INACTIVE, true)
    , d_info(info)
    , d_displayRenderer(accessor.window(), accessor.resourceManager())
    , d_container(
        (float)accessor.window()->width()/4.0f,
        {10.0, 10.0},
        10.0f
    )
    , d_image(accessor.resourceManager()->loadTexture("Resources/Textures/Space.PNG"))
    , d_text({"Sprocket, now only $20!"})
{
    using namespace Sprocket;
    auto resourceManager = accessor.resourceManager();

    d_container.position({10.0f, 10.0f});
    d_container.base().colour = {0.07f, 0.07f, 0.07f};
    d_container.base().roundness = 0.081f;
    d_container.addProperty<Draggable>();

    //d_container.addProperty<VerticalConstraint>(VerticalConstraint::Type::TOP, 10.0f);
    //d_container.addProperty<HorizontalConstraint>(HorizontalConstraint::Type::RIGHT, 10.0f);

    d_image.position({0.0f, 100.0f});
    d_image.addProperty<Draggable>();

    auto topSlider = d_container.add<Slider>(300.0f, 50.0f);
    setSliderAttrs(topSlider);

    auto roundnessSlider = d_container.add<Slider>(300.0f, 50.0f);
    setSliderAttrs(roundnessSlider);

    roundnessSlider->setCallback([&](float val) {
        d_image.base().roundness = val;
    });
    roundnessSlider->setValue(0.6f);

    auto chattyButton = d_container.add<Button>(
        50.0f, 50.0f, 0.5f, 0.55f, 0.45f
    );

    setButtonAttrs(chattyButton);

    chattyButton->setUnclickCallback([&]() {
        SPKT_LOG_WARN("Have a great day!");
        d_accessor.setActiveScene("Main Menu");
    });

    auto cameraSwitchButton = d_container.add<Button>(
        50.0f, 50.0f, 0.5f, 0.55f, 0.45f
    );

    setButtonAttrs(cameraSwitchButton);

    cameraSwitchButton->setUnclickCallback([&]() {  //unclick
        if (d_info->cameraIsFirst) {
            d_info->camera = &d_info->thirdCamera;
        }
        else {
            d_info->camera = &d_info->firstCamera;
        }
        d_info->cameraIsFirst = !d_info->cameraIsFirst;
    });

    auto textBox = d_container.add<TextBox>(
        300.0f, 50.0f, "Text box!"
    );
    
    textBox->base().colour = {0.15625f, 0.15625f, 0.15625f};
    textBox->text().font = Sprocket::Font::CALIBRI;

    topSlider->setCallback([textBox](float val) {
        textBox->text().size = 24.0f + val * 60.0f;
    });

    topSlider->setValue(0.0f);

    auto palette = d_container.add<ColourPalette>(resourceManager, 300.0f, 300.0f);
    palette->base().colour = {0.15625f, 0.15625f, 0.15625f};
    palette->setCallback([&](const Maths::vec3& colour) {
        for (auto& light : d_info->lights) {
            light.colour = colour;
        }
    });
}

bool UILayer::handleEventImpl(const Sprocket::Event& event)
{
    if (auto e = event.as<Sprocket::KeyboardButtonPressedEvent>()) {
        if (e->key() == Sprocket::Keyboard::ESC) {
            d_info->paused = !d_info->paused;
            d_accessor.window()->setCursorVisibility(d_info->paused);
            return true;
        }
        else if (e->key() == Sprocket::Keyboard::E) {
            d_image.active(!d_image.active());
            return true;
        }
    }

    if (d_status == Status::NORMAL) {
        if (d_image.handleEvent(d_accessor.window(), event)) {
            return true;
        }
        if (d_container.handleEvent(d_accessor.window(), event)) {
            return true;
        }
        
    }

    return false;
}

void UILayer::updateImpl()
{
    d_status = d_info->paused ? Status::NORMAL : Status::INACTIVE;
    d_displayRenderer.update();

    if (d_status == Status::NORMAL) {
        d_container.update(d_accessor.window());
        d_image.update(d_accessor.window());
    }
}

void UILayer::drawImpl()
{
    d_container.draw(&d_displayRenderer);
    d_image.draw(&d_displayRenderer);
    d_displayRenderer.draw(d_text);
}