#pragma once
#include "Window.h"
#include "Texture.h"
#include "DevUI.h"

#include <string>

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

// A wrapper for ImGui::Image that uses Sprocket types.
void Image(const Texture& image,
           const Maths::vec2& size,
           const Maths::vec2& uv0 = {0, 1},
           const Maths::vec2& uv1 = {1, 0},
           const Maths::vec4& tintCol = {1, 1, 1, 1},
           const Maths::vec4& borderCol = {0, 0, 0, 0});

// Adds the Gizmo to the current panels draw list
void SetGuizmo();
void GuizmoSettings(
    GizmoMode& mode,
    GizmoCoords& coords,
    bool& useSnap,
    Maths::vec3& snap);

// Displays a quaternion in Euler angles form.
void Euler(const std::string& name, Maths::quat* q);

}
}