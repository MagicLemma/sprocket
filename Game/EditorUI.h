#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"

class EditorUI : public Sprocket::Layer
{
    WorldLayer* d_worldLayer;

    Sprocket::DevGUI d_editorUI;
    Sprocket::DevGUIRenderer d_editorUIRenderer;

public:
    EditorUI(const Sprocket::CoreSystems& core, WorldLayer* worldLayer);

    bool handleEventImpl(const Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;

    friend class EscapeMenu;
};