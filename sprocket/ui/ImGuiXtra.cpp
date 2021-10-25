#include "ImGuiXtra.h"

#include <sprocket/graphics/texture.h>
#include <sprocket/utility/file_browser.h>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

namespace spkt {
namespace ImGuiXtra {
namespace {

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        auto* str = static_cast<std::string*>(data->UserData);
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    return 0;
}

}

void File(const std::string& name,
          window* window,
          std::string* file,
          const std::string& filter)
{
    ImGui::PushID(std::hash<std::string>{}(name));
    if (ImGui::Button(name.c_str())) {
        auto newFile = open_file(window, filter.c_str());
        if (newFile.has_value()) {
            *file = newFile.value();
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
    ImGui::InputText("##", nameStr, IM_ARRAYSIZE(nameStr));
    text = std::string(nameStr);
}

void Text(const std::string& text)
{
    ImGui::Text(text.c_str());
}

void Image(const texture& image, const glm::vec2& size)
{
    ImGui::Image(
        (ImTextureID)(intptr_t)image.id(),
        {size.x, size.y},
        {0, 1},
        {1, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0}
    );
}

void Image(const texture& image, float size)
{
    Image(image, {image.aspect_ratio() * size, size});
}

void Image(const texture& image)
{
    Image(image, {(float)image.width(), (float)image.height()});
}

void GuizmoSettings(
    ImGuizmo::OPERATION& operation,
    ImGuizmo::MODE& mode,
    bool& use_snap,
    glm::vec3& snap)
{
    if (ImGui::RadioButton("Translate", operation == ImGuizmo::OPERATION::TRANSLATE)) {
        operation = ImGuizmo::OPERATION::TRANSLATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", operation == ImGuizmo::OPERATION::ROTATE)) {
        operation = ImGuizmo::OPERATION::ROTATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", operation == ImGuizmo::OPERATION::SCALE)) {
        operation = ImGuizmo::OPERATION::SCALE;
    }

    if (ImGui::RadioButton("World", mode == ImGuizmo::MODE::WORLD)) {
        mode = ImGuizmo::MODE::WORLD;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Local", mode == ImGuizmo::MODE::LOCAL)) {
        mode = ImGuizmo::MODE::LOCAL;
    }
    ImGui::Checkbox("", &use_snap);
    ImGui::SameLine();
    if (operation == ImGuizmo::OPERATION::TRANSLATE) {
        ImGui::InputFloat3("Snap", &snap.x);
    }
    else {
        ImGui::InputFloat("Snap", &snap.x);
    }
}

void Guizmo(
    glm::mat4* matrix,
    const glm::mat4& view,
    const glm::mat4& projection,
    ImGuizmo::OPERATION operation,
    ImGuizmo::MODE mode)
{
    float rw = ImGui::GetWindowWidth();
    float rh = ImGui::GetWindowHeight();
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);

    ImGuizmo::Manipulate(
        glm::value_ptr(view),
        glm::value_ptr(projection),
        operation,
        mode,
        glm::value_ptr(*matrix)
    );
}

glm::vec2 GetMousePosWindowCoords()
{
    auto mouse = ImGui::GetMousePos();
    auto topLeft = ImGui::GetWindowPos();
    float titleBarHeight = ImGui::GetFontSize() + 2 * ImGui::GetStyle().FramePadding.y;
    topLeft.y += titleBarHeight;
    return {mouse.x - topLeft.x, mouse.y - topLeft.y};
}

void Euler(const std::string& name, glm::quat* q)
{
    glm::vec3 euler = glm::eulerAngles(*q);
    if (ImGui::DragFloat3("Orientation", &euler.x, 0.01f)) {
        *q = glm::quat(euler);
    }
}

bool MultilineTextModifiable(const std::string& label, std::string* text)
{
    return ImGui::InputTextMultiline(
        label.c_str(),
        (char*)text->c_str(),
        text->capacity() + 1,
        ImVec2(500, 500),
        ImGuiInputTextFlags_CallbackResize,
        InputTextCallback,
        text
    );
}

}
}