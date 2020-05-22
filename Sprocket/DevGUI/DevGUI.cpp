#include "DevGUI.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "WindowEvent.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Sprocket {
namespace {

void setBackendFlags(ImGuiIO& io)
{
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
}

void setClipboardCallbacks(ImGuiIO& io, Window* window)
{
    io.SetClipboardTextFn = [](void* user_data, const char* text) {
        Sprocket::Window* w = static_cast<Sprocket::Window*>(user_data);
        w->setClipboardData(text);
    };

    io.GetClipboardTextFn = [](void* user_data) {
        Sprocket::Window* w = static_cast<Sprocket::Window*>(user_data);
        return w->getClipboardData();
    };

    io.ClipboardUserData = window;
}

void setKeyMappings(ImGuiIO& io)
{
    io.KeyMap[ImGuiKey_Tab] =         Keyboard::TAB;
    io.KeyMap[ImGuiKey_LeftArrow] =   Keyboard::LEFT_ARROW;
    io.KeyMap[ImGuiKey_RightArrow] =  Keyboard::RIGHT_ARROW;
    io.KeyMap[ImGuiKey_UpArrow] =     Keyboard::UP_ARROW;
    io.KeyMap[ImGuiKey_DownArrow] =   Keyboard::DOWN_ARROW;
    io.KeyMap[ImGuiKey_PageUp] =      Keyboard::PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] =    Keyboard::PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] =        Keyboard::HOME;
    io.KeyMap[ImGuiKey_End] =         Keyboard::END;
    io.KeyMap[ImGuiKey_Insert] =      Keyboard::INSERT;
    io.KeyMap[ImGuiKey_Delete] =      Keyboard::DEL;
    io.KeyMap[ImGuiKey_Backspace] =   Keyboard::BACKSPACE;
    io.KeyMap[ImGuiKey_Space] =       Keyboard::SPACE;
    io.KeyMap[ImGuiKey_Enter] =       Keyboard::ENTER;
    io.KeyMap[ImGuiKey_Escape] =      Keyboard::ESC;
    io.KeyMap[ImGuiKey_KeyPadEnter] = Keyboard::KEYPAD_ENTER;
    io.KeyMap[ImGuiKey_A] =           Keyboard::A;
    io.KeyMap[ImGuiKey_C] =           Keyboard::C;
    io.KeyMap[ImGuiKey_V] =           Keyboard::V;
    io.KeyMap[ImGuiKey_X] =           Keyboard::X;
    io.KeyMap[ImGuiKey_Y] =           Keyboard::Y;
    io.KeyMap[ImGuiKey_Z] =           Keyboard::Z;
}

}

DevGUI::DevGUI(Window* window)
    : d_window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();    
    setBackendFlags(io);
    setClipboardCallbacks(io, window);
    setKeyMappings(io);
}

DevGUI::~DevGUI()
{
    
}

void DevGUI::update(float dt)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = dt;
    io.DisplaySize = ImVec2((float)d_window->width(),
                            (float)d_window->height());
}

bool DevGUI::handleEvent(Event& event)
{
    ImGuiIO& io = ImGui::GetIO();

    if (auto e = event.as<MouseButtonPressedEvent>()) {
        io.MouseDown[e->button()] = true;
        if (ImGui::IsAnyWindowHovered() || ImGuizmo::IsOver()) {
            event.consume();
            return false;
        }
    }
    
    else if (auto e = event.as<MouseButtonReleasedEvent>()) {
        io.MouseDown[e->button()] = false;
        if (ImGui::IsAnyWindowHovered() || ImGuizmo::IsOver()) {
            event.consume();
            return false;
        }
    }

    else if (auto e = event.as<MouseMovedEvent>()) {
        io.MousePos = ImVec2(e->xPos(), e->yPos());
        if (ImGui::IsAnyWindowHovered() || ImGuizmo::IsOver()) {
            event.consume();
            return false;
        }
    }

    else if (auto e = event.as<MouseScrolledEvent>()) {
        io.MouseWheel += e->yOffset();
        io.MouseWheelH += e->xOffset();
        if (ImGui::IsAnyWindowHovered() || ImGuizmo::IsOver()) {
            return false;
        }
    }

    else if (auto e = event.as<WindowResizeEvent>()) {
        io.DisplaySize = ImVec2((float)e->width(), (float)e->height());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    }

    else if (auto e = event.as<KeyboardButtonPressedEvent>()) {
        io.KeysDown[e->key()] = true;
        io.KeyCtrl = e->mods() & KeyModifier::CTRL;
        io.KeyShift = e->mods() & KeyModifier::SHIFT;
        io.KeyAlt = e->mods() & KeyModifier::ALT;
        io.KeySuper = e->mods() & KeyModifier::SUPER;
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) {
            return true;
        }
    }

    else if (auto e = event.as<KeyboardButtonReleasedEvent>()) {
        io.KeysDown[e->key()] = false;
        io.KeyCtrl = e->mods() & KeyModifier::CTRL;
        io.KeyShift = e->mods() & KeyModifier::SHIFT;
        io.KeyAlt = e->mods() & KeyModifier::ALT;
        io.KeySuper = e->mods() & KeyModifier::SUPER;
    }

    else if (auto e = event.as<KeyboardKeyTypedEvent>()) {
        if (e->key() > 0 && e->key() < 0x10000) {
            io.AddInputCharacter((unsigned short)e->key());
        }
    }

    return false;
}

}