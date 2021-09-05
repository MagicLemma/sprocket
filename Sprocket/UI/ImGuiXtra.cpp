#include "ImGuiXtra.h"

#include <Sprocket/Graphics/Texture.h>
#include <Sprocket/Utility/FileBrowser.h>

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
    ImGui::InputText("##", nameStr, IM_ARRAYSIZE(nameStr));
    text = std::string(nameStr);
}

void Text(const std::string& text)
{
    ImGui::Text(text.c_str());
}

void Image(const texture& image,
           const glm::vec2& size,
           const glm::vec2& uv0,
           const glm::vec2& uv1,
           const glm::vec4& tintCol,
           const glm::vec4& borderCol)
{
    ImGui::Image(
        (ImTextureID)(intptr_t)image.id(),
        {size.x, size.y},
        {uv0.x, uv0.y},
        {uv1.x, uv1.y},
        {tintCol.r, tintCol.g, tintCol.b, tintCol.a},
        {borderCol.r, borderCol.g, borderCol.b, borderCol.a}
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
    ImGuizmo::OPERATION& mode,
    ImGuizmo::MODE& coords,
    bool& useSnap,
    glm::vec3& snap)
{
    if (ImGui::RadioButton("Translate", mode == ImGuizmo::OPERATION::TRANSLATE)) {
        mode = ImGuizmo::OPERATION::TRANSLATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mode == ImGuizmo::OPERATION::ROTATE)) {
        mode = ImGuizmo::OPERATION::ROTATE;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mode == ImGuizmo::OPERATION::SCALE)) {
        mode = ImGuizmo::OPERATION::SCALE;
    }

    if (ImGui::RadioButton("World", coords == ImGuizmo::MODE::WORLD)) {
        coords = ImGuizmo::MODE::WORLD;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Local", coords == ImGuizmo::MODE::LOCAL)) {
        coords = ImGuizmo::MODE::LOCAL;
    }
    ImGui::Checkbox("", &useSnap);
    ImGui::SameLine();
    if (mode == ImGuizmo::OPERATION::TRANSLATE) {
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

bool MultilineTextModifiable(const std::string_view label, std::string* text)
{
    return ImGui::InputTextMultiline(
        label.data(),
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