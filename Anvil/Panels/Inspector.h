#pragma once
#include "DevUI.h"

namespace Sprocket {

class EditorLayer;

class Inspector
{
    ImGuizmo::MODE d_coords = ImGuizmo::MODE::WORLD;
    ImGuizmo::OPERATION d_mode = ImGuizmo::OPERATION::TRANSLATE;
    
    bool        d_useSnap = false;
    Maths::vec3 d_snap = {0.0f, 0.0f, 0.0f};

public:
    void Show(EditorLayer& editor);

};

}