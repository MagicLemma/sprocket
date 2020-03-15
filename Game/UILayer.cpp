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

UILayer::UILayer(std::shared_ptr<BasicSceneInfo> info) 
    : Layer(Status::INACTIVE, true)
    , d_info(info)
    , d_displayRenderer(info->window)
    , d_slider({50.0f, 50.0f}, 300.0f, 50.0f, 0.0f, getSliderAttrs())
    , d_slider2({50.0f, 100.0f}, 300.0f, 50.0f, 0.0f, getSliderAttrs())
{
    d_slider.setCallback([&](float val){
        auto lens = static_cast<Sprocket::PerspectiveLens*>(&d_info->lens);
        lens->fov(70.0f + 50.0f * val);
    });

    d_slider2.setCallback([](float val) {
        SPKT_LOG_INFO("Slider 2 set to {}!", val);
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
    d_slider.handleEvent(window, event);
    d_slider2.handleEvent(window, event);
    return false;
}

void UILayer::updateImpl(Sprocket::Window* window)
{
    d_status = d_info->paused ? Status::NORMAL : Status::INACTIVE;

    if (d_status == Status::NORMAL) {
        d_slider.update(window);
        d_slider2.update(window);
    }
}

void UILayer::drawImpl(Sprocket::Window* window)
{
    d_displayRenderer.draw(d_slider);
    d_displayRenderer.draw(d_slider2);
}