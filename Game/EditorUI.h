#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"

class EditorUI : public Sprocket::Layer
{
    WorldLayer* d_worldLayer;

    Sprocket::DevUI d_ui;

public:
    EditorUI(WorldLayer* worldLayer);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
    void OnRender() override;

    friend class EscapeMenu;
};