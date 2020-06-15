#pragma once
#include "Window.h"
#include "Maths.h"
#include "Shader.h"
#include "Event.h"
#include "Texture.h"
#include "Resources.h"
#include "StreamBuffer.h"

#include <memory>

namespace Sprocket {
namespace DevUI {

struct DevUIData;

enum class GizmoMode { TRANSLATION, ROTATION };
enum class GizmoCoords { WORLD, LOCAL };

class Context
{
    std::shared_ptr<DevUIData> d_impl;

public:
    Context(Window* window);

    void OnEvent(Event& event);
    void OnUpdate(float dt);

    void StartFrame();
    void EndFrame();

    void StartWindow(const std::string& name, bool* open = nullptr, int flags = 0);
    void EndWindow();

    bool StartTreeNode(const std::string& name);
    void EndTreeNode();

    bool Button(const std::string& name);
    bool RadioButton(const std::string& name, bool active);
    bool CollapsingHeader(const std::string& name);
    void Text(const std::string& text);
    void TextModifiable(std::string& text);
    void Checkbox(const std::string& name, bool* value);
    void ColourPicker(const std::string& name, Maths::vec3* colour);
    void SliderFloat(const std::string& name, float* value, float lower, float upper);
    void DragInt(const std::string& name, int* value, int speed = 1);
    void DragFloat(const std::string& name, float* value, float speed = 1.0f);
    void DragFloat3(const std::string& name, Maths::vec3* values, float speed = 1.0f);

    void Gizmo(Maths::mat4* matrix,
               const Maths::mat4& view,
               const Maths::mat4& projection,
               GizmoMode mode,
               GizmoCoords coords);

    void SameLine();
    void Separator();

    void PushID(std::size_t id);
    void PopID();

    void DemoWindow();
};

}
}