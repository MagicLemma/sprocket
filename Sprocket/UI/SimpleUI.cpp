#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"

namespace Sprocket {
namespace SimpleUI {

struct SimpleUIData
{
    Window* window;

    KeyboardProxy keyboard;
    MouseProxy mouse;
};

Context::Context(Window* window)
    : d_impl(std::make_shared<SimpleUIData>())
{
}

void Context::OnEvent(Event& event)
{
    d_impl->keyboard.OnEvent(event);
    d_impl->mouse.OnEvent(event);
}

void Context::OnUpdate(float dt)
{
    d_impl->mouse.OnUpdate();
}

}
}