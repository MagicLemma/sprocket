#include "DevUI.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "WindowEvent.h"
#include "KeyboardCodes.h"
#include "RenderContext.h"
#include "BufferLayout.h"
#include "Types.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <glad/glad.h>

namespace Sprocket {
namespace {

void SetDarkTheme()
{
    auto& colours = ImGui::GetStyle().Colors;
}

void SetBackendFlags()
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void SetClipboardCallbacks(Window* window)
{
    ImGuiIO& io = ImGui::GetIO();
    io.SetClipboardTextFn = [](void* user_data, const char* text) {
        Sprocket::Window* w = static_cast<Sprocket::Window*>(user_data);
        w->SetClipboardData(text);
    };

    io.GetClipboardTextFn = [](void* user_data) {
        Sprocket::Window* w = static_cast<Sprocket::Window*>(user_data);
        return w->GetClipboardData();
    };

    io.ClipboardUserData = window;
}

void SetKeyMappings()
{
    ImGuiIO& io = ImGui::GetIO();
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

// Loads the font atlas and assigns it to ImGui. Returns the pointer
// for the caller to own.
std::unique_ptr<Texture> SetFont(std::string_view font, float size)
{
    ImGuiIO& io = ImGui::GetIO();    
    io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Calibri.ttf", 15.0f);
    unsigned char* data;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
    auto texture = std::make_unique<Texture>(width, height, data);
    io.Fonts->TexID = reinterpret_cast<void*>(texture->Id());
    return texture;
}

}

DevUI::DevUI(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/DevGUI.vert",
               "Resources/Shaders/DevGUI.frag")
    , d_fontAtlas(nullptr)
    , d_blockEvents(true)
{
    ImGui::CreateContext();
    IMGUI_CHECKVERSION();

    SetBackendFlags();
    SetClipboardCallbacks(window);
    SetKeyMappings();
    SetDarkTheme();

    d_fontAtlas = SetFont("Resources/Fonts/Calibri.ttf", 15.0f);

    // Reason: when the viewport isn't docked and we have a selected entity,
    // attempting to move the entity just moved the window.
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    BufferLayout bufferLayout(sizeof(ImDrawVert));
    bufferLayout.AddAttribute(DataType::FLOAT, 2);
    bufferLayout.AddAttribute(DataType::FLOAT, 2);
    bufferLayout.AddAttribute(DataType::UBYTE, 4);
    d_buffer.SetBufferLayout(bufferLayout);
}

void DevUI::OnEvent(Event& event)
{
    ImGuiIO& io = ImGui::GetIO();

    if (auto e = event.As<MouseButtonPressedEvent>()) {    
        if (e->IsConsumed()) { return; }
        io.MouseDown[e->Button()] = true;
    }
    
    else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        io.MouseDown[e->Button()] = false;
    }

    else if (auto e = event.As<MouseMovedEvent>()) {
        io.MousePos = ImVec2(e->XPos(), e->YPos());
        if (ImGui::IsAnyItemHovered()) { e->Consume(); }
    }

    else if (auto e = event.As<MouseScrolledEvent>()) {
        io.MouseWheel += e->YOffset();
        io.MouseWheelH += e->XOffset();
    }

    else if (auto e = event.As<WindowResizeEvent>()) {
        io.DisplaySize = ImVec2((float)e->Width(), (float)e->Height());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    }

    else if (auto e = event.As<KeyboardButtonPressedEvent>()) {
        if (e->IsConsumed()) { return; }
        io.KeysDown[e->Key()] = true;
        io.KeyCtrl  = e->Mods() & KeyModifier::CTRL;
        io.KeyShift = e->Mods() & KeyModifier::SHIFT;
        io.KeyAlt   = e->Mods() & KeyModifier::ALT;
        io.KeySuper = e->Mods() & KeyModifier::SUPER;
    }

    else if (auto e = event.As<KeyboardButtonReleasedEvent>()) {
        io.KeysDown[e->Key()] = false;
        io.KeyCtrl  = e->Mods() & KeyModifier::CTRL;
        io.KeyShift = e->Mods() & KeyModifier::SHIFT;
        io.KeyAlt   = e->Mods() & KeyModifier::ALT;
        io.KeySuper = e->Mods() & KeyModifier::SUPER;
    }

    else if (auto e = event.As<KeyboardKeyTypedEvent>()) {
        if (e->IsConsumed()) { return; }
        if (e->Key() > 0 && e->Key() < 0x10000) {
            io.AddInputCharacter((unsigned short)e->Key());
        }
    }

    if (d_blockEvents) {
        if (event.In<EventCategory::KEYBOARD>() && io.WantCaptureKeyboard) {
            event.Consume();
        }

        if (event.In<EventCategory::MOUSE>() && io.WantCaptureMouse) {
            event.Consume();
        }
    }
}

void DevUI::OnUpdate(double dt)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = (float)dt;
    io.DisplaySize = ImVec2((float)d_window->Width(), (float)d_window->Height());
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
}

void DevUI::StartFrame()
{
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void DevUI::EndFrame()
{
    ImGui::Render();
    
    Sprocket::RenderContext rc;  
    rc.AlphaBlending(true);
    rc.FaceCulling(false);
    rc.ScissorTesting(true);
    rc.DepthTesting(false);

    ImDrawData* drawData = ImGui::GetDrawData();

    auto proj = glm::ortho(0.0f, drawData->DisplaySize.x, drawData->DisplaySize.y, 0.0f);

    d_shader.Bind();
    d_shader.LoadSampler("Texture", 0);
    d_shader.LoadMat4("ProjMtx", proj);

    d_buffer.Bind();
    d_fontAtlas->Bind(0);

    // Render command lists
    int width = (int)drawData->DisplaySize.x;
    int height = (int)drawData->DisplaySize.y;

    for (int n = 0; n < drawData->CmdListsCount; ++n) {
        const ImDrawList* cmd_list = drawData->CmdLists[n];

        // Upload vertex/index buffers
        d_buffer.SetVertexData(
            cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
            cmd_list->VtxBuffer.Data
        );
        d_buffer.SetIndexData(
            cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
            cmd_list->IdxBuffer.Data            
        );

        for (int i = 0; i < cmd_list->CmdBuffer.Size; ++i) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[i];
            const ImVec4& rect = pcmd->ClipRect;

            if (rect.x < width && rect.y < height && rect.z >= 0 && rect.w >= 0) {

                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);

                glScissor((int)rect.x,
                          (int)(height - rect.w),
                          (int)(rect.z - rect.x),
                          (int)(rect.w - rect.y));

                glDrawElements(
                    GL_TRIANGLES,
                    pcmd->ElemCount,
                    GL_UNSIGNED_SHORT,
                    (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx))
                );
            }
        }
    }
}

}