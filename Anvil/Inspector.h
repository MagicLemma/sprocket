#pragma once
#include "DevUI.h"

#include <ImGuizmo.h>

namespace spkt {

class Anvil;

class Inspector
{
    ImGuizmo::MODE d_mode = ImGuizmo::MODE::WORLD;
    ImGuizmo::OPERATION d_operation = ImGuizmo::OPERATION::TRANSLATE;
    
    bool      d_useSnap = false;
    glm::vec3 d_snap = {0.0f, 0.0f, 0.0f};

public:
    void Show(Anvil& editor);

    ImGuizmo::MODE Mode() const { return d_mode; }
    ImGuizmo::OPERATION Operation() const { return d_operation; }
    const float* Snap() const { return d_useSnap ? &d_snap.x : nullptr; }
};

}