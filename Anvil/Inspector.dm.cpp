#include "Inspector.h"
#include "Anvil.h"
#include "ImGuiXtra.h"
#include "Maths.h"
#include "ecs.h"
#include "DevUI.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace spkt {

void Inspector::Show(Anvil& editor)
{
    spkt::registry& registry = editor.active_scene()->Entities();
    spkt::entity entity = editor.selected();

    if (!registry.valid(entity)) {
        if (ImGui::Button("New Entity")) {
            entity = registry.create();
            editor.set_selected(entity);
        }
        return;
    }
    int count = 0;

    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1.0), "ID: %llu", entity);

DATAMATIC_BEGIN
    if (registry.has<{{Comp::name}}>(entity)) {
        auto& c = registry.get<{{Comp::name}}>(entity);
        if (ImGui::CollapsingHeader("{{Comp::display_name}}")) {
            ImGui::PushID(count++);
            {{Attr::inspector_display}};
            {{Comp::inspector_guizmo_settings}}
            if (ImGui::Button("Delete")) { registry.remove<{{Comp::name}}>(entity); }
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
        if (!registry.has<{{Comp::name}}>(entity) && ImGui::Selectable("{{Comp::display_name}}")) {
            {{Comp::name}} c;
            registry.add<{{Comp::name}}>(entity, c);
        }
DATAMATIC_END
        ImGui::EndMenu();
    }
    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        spkt::entity copy = spkt::copy_entity(&editor.active_scene()->Entities(), entity);
        editor.set_selected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        registry.destroy(entity);
        editor.clear_selected();
    }
}
    
}
