#include "ImGuiHelpers.h"
#include "FileBrowser.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Sprocket {
namespace ImGuiXtra {

void File(const std::string& name,
          Window* window,
          std::string* file,
          const char* filter)
{
    ImGui::PushID(std::hash<std::string>{}(name));
    if (ImGui::Button(name.c_str())) {
        auto newFile = OpenFile(window, filter);
        if (!newFile.empty()) {
            *file = newFile;
        }
    }
    ImGui::SameLine();
    ImGui::Text(file->c_str());
    ImGui::PopID();
}

void TextModifiable(std::string& text)
{
    char nameStr[128] = "";
    std::memcpy(nameStr, text.c_str(), std::strlen(text.c_str()));
    ImGui::InputText("", nameStr, IM_ARRAYSIZE(nameStr));
    text = std::string(nameStr);
}

void Image(const Texture& image,
           const Maths::vec2& size,
           const Maths::vec2& uv0,
           const Maths::vec2& uv1,
           const Maths::vec4& tintCol,
           const Maths::vec4& borderCol)
{
    ImGui::Image(
        (ImTextureID)(intptr_t)image.Id(),
        {size.x, size.y},
        {uv0.x, uv0.y},
        {uv1.x, uv1.y},
        {tintCol.r, tintCol.g, tintCol.b, tintCol.a},
        {borderCol.r, borderCol.g, borderCol.b, borderCol.a}
    );
}

void SetGuizmo()
{
    float rw = ImGui::GetWindowWidth();
    float rh = ImGui::GetWindowHeight();
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
}

}
}