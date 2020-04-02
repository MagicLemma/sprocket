#pragma once
#include <Sprocket.h>

#include "WorldSceneInfo.h"

#include <memory>

class UILayer : public Sprocket::Layer
{
    std::shared_ptr<BasicSceneInfo> d_info;
    Sprocket::DisplayRenderer d_displayRenderer;
    Sprocket::Container d_container;
    Sprocket::Image d_image;
    Sprocket::Text d_text;

public:
    UILayer(Sprocket::Accessor& accessor, std::shared_ptr<BasicSceneInfo> info);

    bool handleEventImpl(const Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;
};