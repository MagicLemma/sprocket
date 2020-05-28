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

void Context::handleEvent(Event& event)
{
    d_impl->keyboard.handleEvent(event);
    d_impl->mouse.handleEvent(event);
}

void Context::update(float dt)
{
    d_impl->mouse.update();
}

}
}