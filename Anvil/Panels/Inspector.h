#pragma once
#include "DevUI.h"

namespace Sprocket {

class EditorLayer;

class Inspector
{
    DevUI::GizmoCoords d_coords = DevUI::GizmoCoords::WORLD;
    DevUI::GizmoMode d_mode = DevUI::GizmoMode::TRANSLATION;

public:
    void Show(EditorLayer& editor);

};

}