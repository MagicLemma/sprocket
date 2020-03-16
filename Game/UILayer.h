#pragma once
#include <Sprocket.h>

#include "WorldSceneInfo.h"

#include <memory>

class UILayer : public Sprocket::Layer
{
    std::shared_ptr<BasicSceneInfo> d_info;

    Sprocket::DisplayRenderer d_displayRenderer;

    Sprocket::Slider d_slider;
    Sprocket::Slider d_slider2;
    Sprocket::Button d_button;
    Sprocket::Button d_button2;

public:
    UILayer(std::shared_ptr<BasicSceneInfo> info);

    bool handleEventImpl(Sprocket::Window* window, const Sprocket::Event& event) override;
    void updateImpl(Sprocket::Window* window) override;
    void drawImpl(Sprocket::Window* window) override;
};