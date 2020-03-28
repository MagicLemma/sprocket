#include "UILayer.h"

Sprocket::SliderAttributes getSliderAttrs()
{
    Sprocket::SliderAttributes attrs;
    attrs.backgroundColour = {0.15625f, 0.15625f, 0.15625f};
    attrs.barColour = {0.3f, 0.3f, 0.3f};
    attrs.pickerColour = {0.926f, 0.496f, 0.0f};

    attrs.barHeight = 0.1f;
    attrs.pickerWidth = 0.025f;

    return attrs;
}

Sprocket::ButtonAttributes getButtonAttrs()
{
    Sprocket::ButtonAttributes attrs;
    attrs.backgroundColour = {0.15625f, 0.15625f, 0.15625f};
    attrs.backgroundColourHovered = {0.15625f, 0.15625f, 0.15625f};
    attrs.backgroundColourClicked = {0.15625f, 0.15625f, 0.15625f};

    attrs.buttonColour = {0.926f, 0.496f, 0.0f};
    attrs.buttonColourHovered = {0.926f, 0.63281f, 0.3242f};
    attrs.buttonColourClicked = {0.324f, 0.90625f, 0.5352f};

    attrs.buttonWidth = 0.5f;
    attrs.buttonHeight = 0.5f;

    return attrs;
}

Sprocket::ContainerAttributes getContainerAttrs()
{
    Sprocket::ContainerAttributes attrs;
    attrs.backgroundColour = {0.07f, 0.07f, 0.07f};

    return attrs;
}

Sprocket::TextBoxAttributes getTextBoxAttrs()
{
    Sprocket::TextBoxAttributes attrs{};
    attrs.backgroundColour = {0.15625f, 0.15625f, 0.15625f};
    return attrs;
}

UILayer::UILayer(std::shared_ptr<BasicSceneInfo> info) 
    : Layer(Status::INACTIVE, true)
    , d_info(info)
    , d_displayRenderer(info->window)
    , d_container(
        (float)info->window->width()/4.0f,
        {10.0, 10.0},
        10.0f,
        getContainerAttrs()
    )
    , d_image(Sprocket::Texture("Resources/Textures/Space.PNG"))
    , d_text({
        "Sprocket, now only $20!",
        Sprocket::Font::CALIBRI,
        24.0f,
        Sprocket::Maths::vec3{0.9f, 0.6f, 0.3f},
        Sprocket::Maths::vec2{100.f, 100.0f}
    })
{
    using namespace Sprocket;

    d_container.position({10.0f, 10.0f});
    d_container.addProperty<VerticalConstraint>(VerticalConstraint::Type::TOP, 10.0f);
    d_container.addProperty<HorizontalConstraint>(HorizontalConstraint::Type::RIGHT, 10.0f);

    d_image.position({0.0f, 100.0f});
    d_image.addProperty<Draggable>();

    d_container.backgroundVisuals().roundness = 0.081f;

    auto topSlider = d_container.add<Slider>(
        300.0f, 50.0f, 0.0f, getSliderAttrs()
    );

    topSlider->setCallback([&](float val) {
        d_text.size = 24.0f + val * 60.0f;
    });

    auto roundnessSlider = d_container.add<Slider>(
        300.0f, 50.0f, 0.0f, getSliderAttrs()
    );

    roundnessSlider->setCallback([&](float val) {
        d_image.backgroundVisuals().roundness = val;
    });

    auto chattyButton = d_container.add<Button>(
        50.0f, 50.0f, getButtonAttrs()
    );

    chattyButton->setUnclickCallback([&]() {
        SPKT_LOG_WARN("Have a great day!");
        d_image.active(!d_image.active());
        d_image.backgroundVisuals().greyscale = !d_image.backgroundVisuals().greyscale;
    });

    auto cameraSwitchButton = d_container.add<Button>(
        50.0f, 50.0f, getButtonAttrs()
    );

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
        300.0f, 50.0f, "Text box!", getTextBoxAttrs()
    );

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
        d_container.update(window);
        d_image.update(window);
    }
}

void UILayer::drawImpl(Sprocket::Window* window)
{
    d_container.draw(&d_displayRenderer);
    d_image.draw(&d_displayRenderer);
    d_displayRenderer.draw(d_text);
}