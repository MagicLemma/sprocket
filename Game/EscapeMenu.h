#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"
#include "EditorUI.h"

#include <memory>

class EscapeMenu : public Sprocket::Layer
{
    WorldLayer* d_worldLayer;
    EditorUI* d_editorUi;
    
    Sprocket::DisplayRenderer d_displayRenderer;
    Sprocket::Container d_container;
    Sprocket::TextBox* d_text;

public:
    EscapeMenu(const Sprocket::CoreSystems& core,
               WorldLayer* worldLayer,
               EditorUI* editorUi);

    void handleEventImpl(Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;
};