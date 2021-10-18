#pragma once
#include <imgui.h> // Must be included before ImGuizmo
#include <Sprocket/Vendor/ImGuizmo/ImGuizmo.h>

#include <glm/glm.hpp>

namespace anvil {
    class app;
}

class Inspector
{
    ImGuizmo::MODE d_mode = ImGuizmo::MODE::WORLD;
    ImGuizmo::OPERATION d_operation = ImGuizmo::OPERATION::TRANSLATE;
    
    bool      d_useSnap = false;
    glm::vec3 d_snap = {0.0f, 0.0f, 0.0f};

public:
    void Show(anvil::app& editor);

    ImGuizmo::MODE Mode() const { return d_mode; }
    ImGuizmo::OPERATION Operation() const { return d_operation; }
    const float* Snap() const { return d_useSnap ? &d_snap.x : nullptr; }
};