#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"

#include <memory>

class UILayer : public Sprocket::Layer
{
    WorldLayer* d_worldLayer;
    
    Sprocket::DisplayRenderer d_displayRenderer;
    Sprocket::Container d_container;
    Sprocket::Image d_image;
    Sprocket::Text d_text;

public:
    UILayer(Sprocket::Accessor& accessor, WorldLayer* worldLayer);

    bool handleEventImpl(const Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;
};