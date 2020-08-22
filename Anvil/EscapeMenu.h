#pragma once
#include <Sprocket.h>
#include "EditorLayer.h"

#include <memory>

class EscapeMenu : public Sprocket::Layer
{
    WorldLayer* d_worldLayer;
    EditorUI* d_editorUi;
    
    Sprocket::SimpleUI d_ui;

public:
    EscapeMenu(const Sprocket::CoreSystems& core,
               WorldLayer* worldLayer);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
};