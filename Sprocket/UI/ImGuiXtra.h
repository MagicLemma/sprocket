#pragma once
#include "Window.h"
#include "Texture.h"
#include "DevUI.h"
#include "Maths.h"

#include <string>

#include <imgui.h>
#include <ImGuizmo.h>

namespace Sprocket {
namespace ImGuiXtra {

// Create a text box for displaying file paths and a button that
// allows for browsing the file system to set the value of the string.
void File(const std::string& name,
          Window* window,
          std::string* file,
          const char* filter);

// Creates a text box for modifying strings.
void TextModifiable(std::string& text);

// Wrapper that accepts std::strings
void Text(const std::string& text);

// A wrapper for ImGui::Image that uses Sprocket types.
void Image(const std::shared_ptr<Texture>& image,
           const glm::vec2& size,
           const glm::vec2& uv0 = {0, 1},
           const glm::vec2& uv1 = {1, 0},
           const Maths::vec4& tintCol = {1, 1, 1, 1},
           const Maths::vec4& borderCol = {0, 0, 0, 0}
);

// A simplified version of the above where the texture maintains
// aspect ratio. The size is the height in pixels.
void Image(const std::shared_ptr<Texture>& image, float size);

// Adds the Gizmo to the current panels draw list
void SetGuizmo();

void GuizmoSettings(
    ImGuizmo::OPERATION& mode,
    ImGuizmo::MODE& coords,
    bool& useSnap,
    Maths::vec3& snap
);

void Guizmo(
    Maths::mat4* matrix,
    const Maths::mat4& view,
    const Maths::mat4& projection,
    ImGuizmo::OPERATION mode,
    ImGuizmo::MODE coords
);

// Displays a quaternion in Euler angles form.
void Euler(const std::string& name, Maths::quat* q);

// Displays a window for a large amount of text that can be rewritten.
bool MultilineTextModifiable(const std::string_view label, std::string* text);

}
}