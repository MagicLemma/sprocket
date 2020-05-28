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

    void handleEvent(Event& event);
    void update(float dt);

    void startFrame();
    void endFrame();

    void startWindow(const std::string& name, bool* open = nullptr, int flags = 0);
    void endWindow();

    bool startTreeNode(const std::string& name);
    void endTreeNode();

    bool button(const std::string& name);
    bool radioButton(const std::string& name, bool active);
    bool collapsingHeader(const std::string& name);
    void text(const std::string& text);
    void checkbox(const std::string& name, bool* value);
    void sliderFloat(const std::string& name, float* value, float lower, float upper);
    void dragFloat(const std::string& name, float* value, float speed = 1.0f);
    void dragFloat3(const std::string& name, Maths::vec3* values, float speed = 1.0f);

    void gizmo(Maths::mat4* matrix,
               const Maths::mat4& view,
               const Maths::mat4& projection,
               GizmoMode mode,
               GizmoCoords coords);

    void sameLine();
    void separator();

    void pushID(std::size_t id);
    void popID();

    void demoWindow();
};

}
}