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

    Sprocket::Image d_image;
    
    Sprocket::HorizontalConstraint d_hconstraint;
    Sprocket::VerticalConstraint d_vconstraint;
    Sprocket::AspectConstraint d_aconstraint;

public:
    UILayer(std::shared_ptr<BasicSceneInfo> info);

    bool handleEventImpl(Sprocket::Window* window, const Sprocket::Event& event) override;
    void updateImpl(Sprocket::Window* window) override;
    void drawImpl(Sprocket::Window* window) override;
};