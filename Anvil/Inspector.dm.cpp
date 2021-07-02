#include "Inspector.h"
#include "Anvil.h"
#include "ImGuiXtra.h"
#include "Maths.h"
#include "Components.h"
#include "DevUI.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace spkt {

void Inspector::Show(Anvil& editor)
{
    apx::entity e = editor.selected();
    spkt::entity entity{editor.active_scene()->Entities(), e};

    if (!entity.valid()) {
        if (ImGui::Button("New Entity")) {
            auto e = apx::create_from(editor.active_scene()->Entities());
            editor.set_selected(e.entity());
        }
        return;
    }
    int count = 0;

    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1.0), "ID: %llu", entity.entity());

DATAMATIC_BEGIN
    if (entity.has<{{Comp::name}}>()) {
        auto& c = entity.get<{{Comp::name}}>();
        if (ImGui::CollapsingHeader("{{Comp::display_name}}")) {
            ImGui::PushID(count++);
            {{Attr::inspector_display}};
            {{Comp::inspector_guizmo_settings}}
            if (ImGui::Button("Delete")) { entity.remove<{{Comp::name}}>(); }
            ImGui::PopID();
        }
    }

DATAMATIC_END
    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
DATAMATIC_BEGIN
        if (!entity.has<{{Comp::name}}>() && ImGui::Selectable("{{Comp::display_name}}")) {
            {{Comp::name}} c;
            entity.add<{{Comp::name}}>(c);
        }
DATAMATIC_END
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        spkt::entity copy = Loader::Copy(&editor.active_scene()->Entities(), entity);
        editor.set_selected(copy.entity());
    }
    if (ImGui::Button("Delete Entity")) {
        entity.destroy();
        editor.clear_selected();
    }
}
    
}
