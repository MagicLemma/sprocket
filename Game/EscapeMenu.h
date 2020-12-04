#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"

#include <memory>

class EscapeMenu : public Sprocket::Layer
{
    Sprocket::CoreSystems d_core;
    
    WorldLayer* d_worldLayer;

    Sprocket::SimpleUI d_ui;

public:
    EscapeMenu(const Sprocket::CoreSystems& core,
               WorldLayer* worldLayer);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
    void OnRender() override;
};