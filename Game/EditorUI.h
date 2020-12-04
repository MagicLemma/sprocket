#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"

class EditorUI : public Sprocket::Layer
{
    Sprocket::CoreSystems d_core;
    
    WorldLayer* d_worldLayer;

    Sprocket::DevUI d_ui;

public:
    EditorUI(const Sprocket::CoreSystems& core, WorldLayer* worldLayer);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
    void OnRender() override;

    friend class EscapeMenu;
};