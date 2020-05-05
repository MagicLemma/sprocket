#pragma once
#include <Sprocket.h>

#include "WorldLayer.h"

class EditorUI : public Sprocket::Layer
{
    WorldLayer* d_worldLayer;
    
    Sprocket::DisplayRenderer d_displayRenderer;
    Sprocket::Container d_container;
    Sprocket::TextBox* d_entityCount;

    Sprocket::Container d_entityAttrs;
    Sprocket::TextBox* d_positionText;
    Sprocket::TextBox* d_velocityText;
    Sprocket::TextBox* d_speedText;
    Sprocket::Button* d_deleteButton;

public:
    EditorUI(Sprocket::Accessor& accessor, WorldLayer* worldLayer);

    bool handleEventImpl(const Sprocket::Event& event) override;
    void updateImpl() override;
    void drawImpl() override;

    friend class EscapeMenu;
};