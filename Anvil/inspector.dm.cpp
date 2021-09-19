#include "Inspector.h"

#include <Anvil/Anvil.h>

#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/Loader.h>
#include <Sprocket/Scene/meta.h>
#include <Sprocket/UI/DevUI.h>
#include <Sprocket/UI/ImGuiXtra.h>
#include <Sprocket/Utility/Maths.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace {

template <typename T>
struct inspector_display;

DATAMATIC_BEGIN
template <>
struct inspector_display<spkt::{{Comp::name}}>
{
    static void draw(Anvil& editor, spkt::{{Comp::name}}& c)
    {
        {{Attr::inspector_display}};
    }
};

DATAMATIC_END

}

void Inspector::Show(Anvil& editor)
{
    spkt::registry& registry = editor.active_scene()->registry;
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
    if (registry.has<spkt::{{Comp::name}}>(entity)) {
        auto& c = registry.get<spkt::{{Comp::name}}>(entity);
        if (ImGui::CollapsingHeader("{{Comp::display_name}}")) {
            ImGui::PushID(count++);
            inspector_display<spkt::{{Comp::name}}>::draw(editor, c);
            if constexpr (std::is_same_v<spkt::{{Comp::name}}, spkt::Transform3DComponent>) {
                spkt::ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);
            }
            if (ImGui::Button("Delete")) { registry.remove<spkt::{{Comp::name}}>(entity); }
            ImGui::PopID();
        }
    }

DATAMATIC_END
    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
        spkt::for_each_reflect([&]<typename T>(spkt::reflection<T> refl) {
            std::string comp_name{refl.component_name};
            if (!registry.has<T>(entity) && ImGui::Selectable(comp_name.c_str())) {
                registry.add<T>(entity, {});
            }
        });
        ImGui::EndMenu();
    }
    ImGui::Separator();

    ImGui::Separator();
    if (ImGui::Button("Duplicate")) {
        spkt::entity copy = spkt::copy_entity(editor.active_scene()->registry, entity);
        editor.set_selected(copy);
    }
    if (ImGui::Button("Delete Entity")) {
        registry.destroy(entity);
        editor.clear_selected();
    }
}
