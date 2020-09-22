#pragma once
#include "DevUI.h"

namespace Sprocket {

class EditorLayer;

class Inspector
{
    DevUI::GizmoCoords d_coords = DevUI::GizmoCoords::WORLD;
    DevUI::GizmoMode d_mode = DevUI::GizmoMode::TRANSLATION;
    
    bool        d_useSnap = false;
    Maths::vec3 d_snap = {0.0f, 0.0f, 0.0f};

public:
    void Show(EditorLayer& editor);

};

}