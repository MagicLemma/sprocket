#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"

class EditorUI : public Sprocket::Layer
{
    WorldLayer* d_worldLayer;

    Sprocket::DevUI::Context d_ui;

    Sprocket::ModelManager* d_modelManager;

public:
    EditorUI(const Sprocket::CoreSystems& core, WorldLayer* worldLayer);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(float dt) override;

    friend class EscapeMenu;
};