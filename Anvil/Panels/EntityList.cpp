#include "EntityList.h"
#include "EditorLayer.h"

#include <Sprocket.h>

#include <imgui.h>

namespace Sprocket {
namespace {

std::string Name(const Entity& entity)
{
    if (entity.Has<NameComponent>()) {
        return entity.Get<NameComponent>().name;
    }
    return "Entity";
}

Entity AddEntityToList(const Entity& entity)
{
    Entity ret = Entity();

    ImGui::PushID(entity.Id());
    if (ImGui::Button(Name(entity).c_str())) {
        ret = entity;
    }
    ImGui::PopID();
    return ret;
}

bool SubstringCI(const std::string& string, const std::string& substr) {
    auto it = std::search(
        string.begin(), string.end(),
        substr.begin(), substr.end(),
        [] (char c1, char c2) { return std::toupper(c1) == std::toupper(c2); }
    );
    return it != string.end();
}

}

void EntityList::Show(EditorLayer& editor)
{
    bool open = true;

    ImGuiWindowFlags flags = 
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoNavInputs |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize;
    
    if (ImGui::Begin("Entities", &open, flags)) {
        ImGuiXtra::TextModifiable(d_search);
        ImGui::Separator();
        editor.GetScene()->All([&](Entity& entity) {
            if (SubstringCI(Name(entity), d_search)) {
                editor.SetSelected(AddEntityToList(entity));
            }
        });
        ImGui::End();
    }
}

}
