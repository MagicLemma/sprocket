#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

#include <string_view>
#include <string>

namespace spkt {

class window;
class texture;

namespace ImGuiXtra {

// Create a text box for displaying file paths and a button that
// allows for browsing the file system to set the value of the string.
void File(const std::string& name,
          window* window,
          std::string* file,
          const std::string& filter);

// Creates a text box for modifying strings.
void TextModifiable(std::string& text);

// Wrapper that accepts std::strings
void Text(const std::string& text);

// A wrapper for ImGui::Image that uses Sprocket types.
void Image(const texture& image,
           const glm::vec2& size,
           const glm::vec2& uv0 = {0, 1},
           const glm::vec2& uv1 = {1, 0},
           const glm::vec4& tintCol = {1, 1, 1, 1},
           const glm::vec4& borderCol = {0, 0, 0, 0}
);

// A simplified version of the above where the texture maintains
// aspect ratio. The size is the height in pixels.
void Image(const texture& image, float size);

// An even more simple version where the size is the native size of the image.
void Image(const texture& image);

void GuizmoSettings(
    ImGuizmo::OPERATION& mode,
    ImGuizmo::MODE& coords,
    bool& useSnap,
    glm::vec3& snap
);

void Guizmo(
    glm::mat4* matrix,
    const glm::mat4& view,
    const glm::mat4& projection,
    ImGuizmo::OPERATION mode,
    ImGuizmo::MODE coords
);

// Returns the coords of the mouse with respect to the top left of the current
// window.
glm::vec2 GetMousePosWindowCoords();

// Displays a quaternion in Euler angles form.
void Euler(const std::string& name, glm::quat* q);

// Displays a window for a large amount of text that can be rewritten.
bool MultilineTextModifiable(const std::string_view label, std::string* text);

}
}