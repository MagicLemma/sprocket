#include "Keyboard.h"
#include "Log.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Sprocket {
namespace Mouse {

namespace {

bool s_initialised = false;

float s_currentMouseX = 0;
float s_currentMouseY = 0;

std::unordered_map<int, bool> s_pressedState;

void handleEvent(const Event& event)
{
    if (auto e = event.as<MouseMovedEvent>()) {
        s_currentMouseX = e->xPos();
        s_currentMouseY = e->yPos();
    }
    else if (auto e = event.as<MouseButtonPressedEvent>()) {
        s_pressedState[e->button()] = true;
        SPKT_LOG_INFO("Mouse button {} pressed", e->button());
    }
    else if (auto e = event.as<MouseButtonReleasedEvent>()) {
        s_pressedState[e->button()] = false;
        SPKT_LOG_INFO("Mouse button {} released", e->button());
    }
}

}

void init(Window* window)
{
    window->registerCallback([&](const Event& event) {
        handleEvent(event);
    });
    SPKT_LOG_INFO("Initialised Mouse");
    s_initialised = true;
}

glm::vec2 getMousePos()
{
    if (!s_initialised) {
        SPKT_LOG_ERROR("Mouse not initalised! Returned value will not be accurate!");
    }

    return {s_currentMouseX, s_currentMouseY};
}

bool isButtonDown(int button)
{
    if (!s_initialised) {
        SPKT_LOG_ERROR("Mouse not initalised! Returned value will not be accurate!");
    }

    auto it = s_pressedState.find(button);
    if (it == s_pressedState.end()) {
        s_pressedState[button] = false;
        return false;
    }

    return it->second;
}

}
}