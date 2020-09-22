#pragma once
#include "DevUI.h"

namespace Sprocket {

class EditorLayer;

class Inspector
{
    GizmoCoords d_coords = GizmoCoords::WORLD;
    GizmoMode d_mode = GizmoMode::TRANSLATION;
    
    bool        d_useSnap = false;
    Maths::vec3 d_snap = {0.0f, 0.0f, 0.0f};

public:
    void Show(EditorLayer& editor);

};

}