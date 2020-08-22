#pragma once
#include <Sprocket.h>
#include "EditorLayer.h"

#include <memory>

namespace Sprocket {

class EscapeMenu : public Layer
{
    EditorLayer* d_editorLayer;
    SimpleUI d_ui;

public:
    EscapeMenu(const CoreSystems& core, EditorLayer* worldLayer);

    void OnEvent(Event& event) override;
    void OnUpdate(double dt) override;
};

}