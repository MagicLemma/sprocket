#pragma once
#include "Window.h"
#include "Maths.h"
#include "Shader.h"
#include "Event.h"
#include "Texture.h"
#include "Resources.h"
#include "StreamBuffer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include <memory>

namespace Sprocket {

struct DevUIData;

enum class GizmoMode { TRANSLATION, ROTATION, SCALE };
enum class GizmoCoords { WORLD, LOCAL };

ImGuizmo::OPERATION GetMode(GizmoMode mode);
ImGuizmo::MODE GetCoords(GizmoCoords coords);

class DevUI
{
    std::shared_ptr<DevUIData> d_impl;
    bool d_blockEvents = true;

public:
    DevUI(Window* window);

    void OnEvent(Event& event);
    void OnUpdate(double dt);

    void BlockEvents(bool val) { d_blockEvents = val; }

    void StartFrame();
    void EndFrame();

    bool StartTreeNode(const std::string& name);
    void EndTreeNode();

    void MultilineTextModifiable(const std::string_view label, std::string &text);

    void Gizmo(Maths::mat4* matrix,
               const Maths::mat4& view,
               const Maths::mat4& projection,
               GizmoMode mode,
               GizmoCoords coords);

    void PushID(std::size_t id);
    void PopID();

    void DemoWindow();
};

}