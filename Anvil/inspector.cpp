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

#include <type_traits>

namespace {

template <typename T>
struct inspector_display;

template <typename T, bool Savable, bool Scriptable>
void imgui_display_attribute(
    Anvil& editor,
    spkt::attribute_reflection<T, Savable, Scriptable>& attr)
{
    if constexpr (std::is_same_v<T, std::string>) {
        if (auto it = attr.metadata.find("file"); it != attr.metadata.end()) {
            spkt::ImGuiXtra::File(std::string{attr.display_name}, editor.window(), attr.value, std::string{it->second}.c_str());
        } else {
            spkt::ImGuiXtra::TextModifiable(*attr.value);
        }
    }
}

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
    spkt::for_each_reflect([&]<typename T>(spkt::reflection<T> refl) {
        if (registry.has<T>(entity)) {
            auto& c = registry.get<T>(entity);
            if (ImGui::CollapsingHeader(refl.name)) {
                ImGui::PushID(count++);
                refl.attributes(c, [&](auto&& attr) {
                    imgui_display_attribute(editor, attr);
                });
                if constexpr (std::is_same_v<T, spkt::Transform3DComponent>) {
                    spkt::ImGuiXtra::GuizmoSettings(d_operation, d_mode, d_useSnap, d_snap);
                }
                if (ImGui::Button("Delete")) { registry.remove<T>(entity); }
                ImGui::PopID();
            }
        }
    });

    ImGui::Separator();

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("missing_components_list");
    }

    if (ImGui::BeginPopup("missing_components_list")) {
        spkt::for_each_reflect([&]<typename T>(spkt::reflection<T> refl) {
            if (!registry.has<T>(entity) && ImGui::Selectable(refl.name)) {
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
