#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"

#include <memory>

class EscapeMenu : public Sprocket::Layer
{
    WorldLayer* d_worldLayer;

    Sprocket::SimpleUI d_ui;
    
    Sprocket::DisplayRenderer d_displayRenderer;
    Sprocket::Container d_container;
    Sprocket::TextBox* d_text;

public:
    EscapeMenu(const Sprocket::CoreSystems& core,
               WorldLayer* worldLayer);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
};