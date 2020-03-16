#include "UILayer.h"
#include "Slider.h"
#include "Log.h"
#include "PerspectiveLens.h"

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

UILayer::UILayer(std::shared_ptr<BasicSceneInfo> info) 
    : Layer(Status::INACTIVE, true)
    , d_info(info)
    , d_displayRenderer(info->window)
    , d_slider({50.0f, 50.0f}, 300.0f, 50.0f, 0.0f, getSliderAttrs())
    , d_slider2({50.0f, 100.0f}, 300.0f, 50.0f, 0.0f, getSliderAttrs())
    , d_button({50.0f, 200.0f}, 50.0f, 50.0f, getButtonAttrs())
    , d_button2({100.0f, 200.0f}, 50.0f, 50.0f, getButtonAttrs())
{
    d_slider.setCallback([&](float val){
        d_info->lens.fov(70.0f + 50.0f * val);
    });

    d_slider2.setCallback([](float val) {
        SPKT_LOG_INFO("Slider 2 set to {}!", val);
    });

    d_button.setUnclickCallback([&]() {
        d_info->paused = !d_info->paused;
        d_info->window->setCursorVisibility(d_info->paused);
    });

    d_button2.setUnclickCallback([&]() {
        if (d_info->cameraIsFirst) {
            d_info->camera = &d_info->thirdCamera;
        }
        else {
            d_info->camera = &d_info->firstCamera;
        }
        d_info->cameraIsFirst = !d_info->cameraIsFirst;
    });
}

bool UILayer::handleEventImpl(Sprocket::Window* window, const Sprocket::Event& event)
{
    if (auto e = event.as<Sprocket::KeyboardButtonPressedEvent>()) {
        if (e->key() == Sprocket::Keyboard::ESC) {
            d_info->paused = !d_info->paused;
            window->setCursorVisibility(d_info->paused);
            return true;
        }
    }
    if (d_status == Status::NORMAL) {
        d_slider.handleEvent(window, event);
        d_slider2.handleEvent(window, event);
        d_button.handleEvent(window, event);
        d_button2.handleEvent(window, event);
    }
    return false;
}

void UILayer::updateImpl(Sprocket::Window* window)
{
    d_status = d_info->paused ? Status::NORMAL : Status::INACTIVE;

    if (d_status == Status::NORMAL) {
        d_slider.update(window);
        d_slider2.update(window);
        d_button.update(window);
        d_button2.update(window);
    }
}

void UILayer::drawImpl(Sprocket::Window* window)
{
    d_displayRenderer.draw(d_slider);
    d_displayRenderer.draw(d_slider2);
    d_displayRenderer.draw(d_button);
    d_displayRenderer.draw(d_button2);
}