#include "Inspector.h"
#include "Anvil.h"
#include "ImGuiXtra.h"
#include "Maths.h"
#include "Components.h"
#include "DevUI.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Sprocket {

void Inspector::Show(Anvil& editor)
{
    spkt::entity entity = editor.Selected();

    if (!editor.Selected().valid()) {
        if (ImGui::Button("New Entity")) {
            auto e = apx::create_from(editor.GetScene()->Entities());
            editor.SetSelected(e);
        }
        return;
    }
    int count = 0;

    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1.0), "ID: %llu", entity.entity());

#ifdef DATAMATIC_BLOCK
    if (entity.has<{{Comp.name}}>()) {
        auto& c = entity.get<{{Comp.name}}>();
        if (ImGui::CollapsingHeader("{{Comp.display_name}}")) {
            ImGui::PushID(count++);
            {{Attr.Inspector.Display}};
            {{Comp.Inspector.GuizmoSettings}}
            if (ImGui::Button("Delete")) { entity.remove<{{Comp.name}}>(); }
            ImGui::PopID();
        }
    }

#endif
    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
#ifdef DATAMATIC_BLOCK
        if (!entity.has<{{Comp.name}}>() && ImGui::Selectable("{{Comp.display_name}}")) {
            {{Comp.name}} c;
            entity.add<{{Comp.name}}>(c);
        }
#endif
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        spkt::entity copy = Loader::Copy(&editor.GetScene()->Entities(), entity);
        editor.SetSelected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        entity.destroy();
        editor.ClearSelected();
    }
}
    
}
