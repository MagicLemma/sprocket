#include "DevUI.h"
#include "Events.h"
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
    // Colour scheme by github.com/Raikiri
    auto& style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.PopupRounding = 5.0f;
    
    ImVec4* colours = style.Colors;
    colours[ImGuiCol_Text] = ImVec4(0.94f, 0.96f, 0.98f, 1.00f);
    colours[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colours[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colours[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colours[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colours[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colours[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colours[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colours[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colours[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colours[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    colours[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colours[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colours[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colours[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    colours[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colours[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colours[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    colours[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colours[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colours[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colours[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colours[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colours[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colours[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colours[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colours[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colours[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colours[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colours[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colours[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colours[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colours[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colours[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colours[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colours[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colours[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colours[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colours[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colours[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colours[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colours[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colours[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colours[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colours[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colours[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colours[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colours[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
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

void DevUI::OnEvent(ev::Event& event)
{
    ImGuiIO& io = ImGui::GetIO();

    if (auto data = event.get_if<ev::MouseButtonPressed>()) {    
        if (event.is_consumed()) { return; }
        io.MouseDown[data->button] = true;
        if (d_blockEvents && io.WantCaptureMouse) { event.consume(); }
    }
    
    else if (auto data = event.get_if<ev::MouseButtonReleased>()) {
        io.MouseDown[data->button] = false;
    }

    else if (auto data = event.get_if<ev::MouseMoved>()) {
        io.MousePos = ImVec2(data->x_pos, data->y_pos);
        if (ImGui::IsAnyItemHovered()) { event.consume(); }
        if (d_blockEvents && io.WantCaptureMouse) { event.consume(); }
    }

    else if (auto data = event.get_if<ev::MouseScrolled>()) {
        io.MouseWheel += data->y_offset;
        io.MouseWheelH += data->x_offset;
        if (d_blockEvents && io.WantCaptureMouse) { event.consume(); }
    }

    else if (auto data = event.get_if<ev::WindowResize>()) {
        io.DisplaySize = ImVec2((float)data->width, (float)data->height);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    }

    else if (auto data = event.get_if<ev::KeyboardButtonPressed>()) {
        if (event.is_consumed()) { return; }
        io.KeysDown[data->key] = true;
        io.KeyCtrl  = data->mods & KeyModifier::CTRL;
        io.KeyShift = data->mods & KeyModifier::SHIFT;
        io.KeyAlt   = data->mods & KeyModifier::ALT;
        io.KeySuper = data->mods & KeyModifier::SUPER;
        if (d_blockEvents && io.WantCaptureKeyboard) { event.consume(); }
    }

    else if (auto data = event.get_if<ev::KeyboardButtonReleased>()) {
        io.KeysDown[data->key] = false;
        io.KeyCtrl  = data->mods & KeyModifier::CTRL;
        io.KeyShift = data->mods & KeyModifier::SHIFT;
        io.KeyAlt   = data->mods & KeyModifier::ALT;
        io.KeySuper = data->mods & KeyModifier::SUPER;
    }

    else if (auto data = event.get_if<ev::KeyboardTyped>()) {
        if (event.is_consumed()) { return; }
        if (data->key > 0 && data->key < 0x10000) {
            io.AddInputCharacter((unsigned short)data->key);
        }
        if (d_blockEvents && io.WantCaptureKeyboard) { event.consume(); }
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