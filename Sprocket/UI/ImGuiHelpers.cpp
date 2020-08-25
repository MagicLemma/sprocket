#include "ImGuiHelpers.h"
#include "FileBrowser.h"

#include <imgui.h>

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

}
}