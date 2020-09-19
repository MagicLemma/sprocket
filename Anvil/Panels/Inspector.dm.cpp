#include "Inspector.h"
#include "EditorLayer.h"
#include "ImGuiXtra.h"
#include "Maths.h"
#include "Components.h"
#include "DevUI.h"

#include <imgui.h>

namespace Sprocket {
namespace {

void ShowGuizmo(
    EditorLayer& editor,
    TransformComponent& c,
    DevUI::GizmoMode mode,
    DevUI::GizmoCoords coords)
{
    if (!editor.IsGameRunning()) {
        auto& camera = editor.GetEditorCamera();
        auto tr = Maths::Transform(c.position, c.orientation, c.scale);
        ImGuizmo::Manipulate(
            Maths::Cast(camera.View()),
            Maths::Cast(camera.Proj()),
            GetMode(mode),
            GetCoords(coords),
            Maths::Cast(tr)
        );
        Maths::Decompose(tr, &c.position, &c.orientation, &c.scale);
    }
}

}

void Inspector::Show(EditorLayer& editor)
{
    Entity entity = editor.Selected();

    if (editor.Selected().Null()) {
        if (ImGui::Button("New Entity")) {
            auto e = editor.GetScene()->NewEntity();
            editor.SetSelected(e);
        }
        return;
    }
    int count = 0;

#ifdef DATAMATIC_BLOCK
    if (entity.Has<{{Comp.Name}}>()) {
        auto& c = entity.Get<{{Comp.Name}}>();
        if (ImGui::CollapsingHeader("{{Comp.DisplayName}}")) {
            ImGui::PushID(count++);
            {{Attr.Inspector.Display}};
            {{Comp.Inspector.GuizmoSettings}}
            if (ImGui::Button("Delete")) { entity.Remove<{{Comp.Name}}>(); }
            ImGui::PopID();
        }
        {{Comp.Inspector.Guizmo}}
    }

#endif
    ImGui::Separator();

    if (ImGui::BeginMenu("Add Component")) {
#ifdef DATAMATIC_BLOCK
        if (!entity.Has<{{Comp.Name}}>() && ImGui::MenuItem("{{Comp.DisplayName}}")) {
            {{Comp.Name}} c;
            entity.Add(c);
        }
#endif
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        Entity copy = Loader::Copy(editor.GetScene(), entity);
        editor.SetSelected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        entity.Kill();
        editor.ClearSelected();
    }
}
    
}
