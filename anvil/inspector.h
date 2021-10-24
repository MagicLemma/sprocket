#pragma once
#include <imgui.h> // Must be included before ImGuizmo
#include <sprocket/Vendor/ImGuizmo/ImGuizmo.h>

#include <glm/glm.hpp>

namespace anvil {

class app;

class inspector
{
    ImGuizmo::MODE      d_mode      = ImGuizmo::MODE::WORLD;
    ImGuizmo::OPERATION d_operation = ImGuizmo::OPERATION::TRANSLATE;
    
    bool      d_use_snap = false;
    glm::vec3 d_snap = {0.0f, 0.0f, 0.0f};

public:
    void show(anvil::app& editor);

    ImGuizmo::MODE mode() const { return d_mode; }
    ImGuizmo::OPERATION operation() const { return d_operation; }
    const float* snap() const { return d_use_snap ? &d_snap.x : nullptr; }
};

}