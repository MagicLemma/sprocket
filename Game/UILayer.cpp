#include "UILayer.h"

void setButtonAttrs(std::shared_ptr<Sprocket::Button> button)
{
    button->background().colour = {0.15625f, 0.15625f, 0.15625f};
    button->backgroundHovered().colour = {0.15625f, 0.15625f, 0.15625f};
    button->backgroundClicked().colour = {0.15625f, 0.15625f, 0.15625f};

    button->button().colour = {0.926f, 0.496f, 0.0f};
    button->buttonHovered().colour = {0.926f, 0.63281f, 0.3242f};
    button->buttonClicked().colour = {0.324f, 0.90625f, 0.5352f};
}

void setSliderAttrs(std::shared_ptr<Sprocket::Slider> slider)
{
    slider->background().colour = {0.15625f, 0.15625f, 0.15625f};
    slider->bar().colour = {0.3f, 0.3f, 0.3f};
    slider->picker().colour = {0.926f, 0.496f, 0.0f};

    slider->bar().width = slider->background().width * 0.9f;
    slider->bar().height = slider->background().height * 0.1f;
    slider->bar().position.x = slider->background().width * (1.0f - 0.9f) / 2.0f;
    slider->bar().position.y = slider->background().height * (1.0f - 0.1f) / 2.0f;

    slider->picker().width = slider->background().width * 0.02f;
    slider->picker().height = slider->background().height * 0.8f;
    slider->picker().position.x = slider->background().width * (1.0f - 0.02f) / 2.0f;
    slider->picker().position.y = slider->background().height * (1.0f - 0.8f) / 2.0f;
}

UILayer::UILayer(std::shared_ptr<BasicSceneInfo> info) 
    : Layer(Status::INACTIVE, true)
    , d_info(info)
    , d_displayRenderer(info->window)
    , d_container(
        (float)info->window->width()/4.0f,
        {10.0, 10.0},
        10.0f
    )
    , d_image(Sprocket::Texture("Resources/Textures/Space.PNG"))
    , d_text({"Sprocket, now only $20!"})
    , d_quad({
        {50.0f, 50.0f}, 50.0f, 50.0f,
        Sprocket::Texture::empty(),
        {1.0, 0.0, 0.0}
    }, {
        {75.0f, 50.0f}, 50.0f, 50.0f,
        Sprocket::Texture::empty(),
        {0.0, 1.0, 0.0}
    }, {
        {50.0f, 50.0f}, 50.0f, 50.0f,
        Sprocket::Texture::empty(),
        {0.0, 0.0, 1.0}
    })
{
    using namespace Sprocket;

    d_container.position({10.0f, 10.0f});
    d_container.background().colour = {0.07f, 0.07f, 0.07f};
    d_container.background().roundness = 0.081f;

    d_container.addProperty<VerticalConstraint>(VerticalConstraint::Type::TOP, 10.0f);
    d_container.addProperty<HorizontalConstraint>(HorizontalConstraint::Type::RIGHT, 10.0f);

    d_image.position({0.0f, 100.0f});
    d_image.addProperty<Draggable>();

    auto topSlider = d_container.add<Slider>(300.0f, 50.0f);
    setSliderAttrs(topSlider);

    auto roundnessSlider = d_container.add<Slider>(300.0f, 50.0f);
    setSliderAttrs(roundnessSlider);

    roundnessSlider->setCallback([&](float val) {
        d_image.background().roundness = val;
    });
    roundnessSlider->setValue(0.6f);

    auto chattyButton = d_container.add<Button>(
        50.0f, 50.0f, 0.5f, 0.55f, 0.45f
    );

    setButtonAttrs(chattyButton);

    chattyButton->setUnclickCallback([&]() {
        SPKT_LOG_WARN("Have a great day!");
        d_image.active(!d_image.active());
        d_image.background().greyscale = !d_image.background().greyscale;
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
    
    textBox->background().colour = {0.15625f, 0.15625f, 0.15625f};
    textBox->text().font = Sprocket::Font::GEORGIA;

    topSlider->setCallback([textBox](float val) {
        textBox->text().size = 24.0f + val * 60.0f;
    });

    topSlider->setValue(0.8f);
}

bool UILayer::handleEventImpl(Sprocket::Window* window, const Sprocket::Event& event)
{
    if (auto e = event.as<Sprocket::KeyboardButtonPressedEvent>()) {
        if (e->key() == Sprocket::Keyboard::ESC) {
            d_info->paused = !d_info->paused;
            window->setCursorVisibility(d_info->paused);
            return true;
        }
        else if (e->key() == Sprocket::Keyboard::E) {
            d_image.active(!d_image.active());
            return true;
        }
    }

    if (d_status == Status::NORMAL) {
        if (d_quad.handleEvent(window, event)) {
            return true;
        }
        if (d_image.handleEvent(window, event)) {
            return true;
        }
        if (d_container.handleEvent(window, event)) {
            return true;
        }
        
    }

    return false;
}

void UILayer::updateImpl(Sprocket::Window* window)
{
    d_status = d_info->paused ? Status::NORMAL : Status::INACTIVE;
    d_displayRenderer.update();

    if (d_status == Status::NORMAL) {
        d_quad.update(window);
        d_container.update(window);
        d_image.update(window);
    }
}

void UILayer::drawImpl(Sprocket::Window* window)
{
    d_container.draw(&d_displayRenderer);
    d_image.draw(&d_displayRenderer);
    d_displayRenderer.draw(d_text);
    d_quad.draw(&d_displayRenderer);
}