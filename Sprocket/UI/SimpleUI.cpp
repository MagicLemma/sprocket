#include "SimpleUI.h"
#include "KeyboardProxy.h"
#include "MouseProxy.h"
#include "MouseCodes.h"

namespace Sprocket {

SimpleUI::SimpleUI(Window* window)
    : d_window(window)
{
}

void SimpleUI::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
}

void SimpleUI::OnUpdate(double dt)
{
    d_mouse.OnUpdate();
}

bool SimpleUI::Button(
    const std::string& name,
    float x, float y,
    float width, float height)
{
    auto mouse = d_mouse.GetMousePos();
    return x < mouse.x && mouse.x < x + width &&
           y < mouse.y && mouse.y < y + height &&
           d_mouse.IsButtonDown(Mouse::LEFT);
}

}