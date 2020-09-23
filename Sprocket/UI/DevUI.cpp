#include "DevUI.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "WindowEvent.h"
#include "KeyboardCodes.h"
#include "RenderContext.h"
#include "BufferLayout.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <glad/glad.h>

namespace ImGuiExtra {
bool  InputTextMultiline(const char* label, 
                         std::string* str, 
                         const ImVec2& size = ImVec2(0, 0), 
                         ImGuiInputTextFlags flags = 0);
}

namespace Sprocket {
namespace {

unsigned int Cast(ImTextureID id)
{
    return (unsigned int)(intptr_t)id;
}

ImTextureID Cast(unsigned int id)
{
    return (ImTextureID)(intptr_t)id;
}

void SetBackendFlags(ImGuiIO& io)
{
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
}

void SetClipboardCallbacks(ImGuiIO& io, Window* window)
{
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

void SetKeyMappings(ImGuiIO& io)
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

void SetFontAtlas(ImGuiIO& io, Texture& fontAtlas)
{
    unsigned char* data;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
    fontAtlas = Texture(width, height, data);
    io.Fonts->TexID = Cast(fontAtlas.Id());
}

}

ImGuizmo::OPERATION GetMode(GizmoMode mode)
{
    switch (mode) {
        case GizmoMode::TRANSLATION: return ImGuizmo::OPERATION::TRANSLATE;
        case GizmoMode::ROTATION:    return ImGuizmo::OPERATION::ROTATE;
        case GizmoMode::SCALE:       return ImGuizmo::OPERATION::SCALE;
        default: {
            SPKT_LOG_ERROR("Unknown GizmoMode!");
            return ImGuizmo::OPERATION::TRANSLATE;
        }
    }
}

ImGuizmo::MODE GetCoords(GizmoCoords coords) 
{
    switch (coords) {
        case GizmoCoords::WORLD: return ImGuizmo::MODE::WORLD;
        case GizmoCoords::LOCAL: return ImGuizmo::MODE::LOCAL;
        default: {
            SPKT_LOG_ERROR("Unknown GizmoCoords!");
            return ImGuizmo::MODE::WORLD;
        }
    }
}

struct DevUIData
{
    ImGuiContext* context;

    // SPROCKET OBJECTS
    Window* window;
    Shader  shader;
    Texture fontAtlas;

    StreamBuffer buffer;
        // Buffer used to store the render data created by ImGui
        // for rendering it.

    DevUIData()
        : shader("Resources/Shaders/DevGUI.vert",
                 "Resources/Shaders/DevGUI.frag")
    {
        BufferLayout bufferLayout(sizeof(ImDrawVert));
        bufferLayout.AddAttribute(DataType::FLOAT, 2);
        bufferLayout.AddAttribute(DataType::FLOAT, 2);
        bufferLayout.AddAttribute(DataType::UBYTE, 4);
        buffer.SetBufferLayout(bufferLayout);
    }
};

DevUI::DevUI(Window* window)
    : d_impl(std::make_shared<DevUIData>())
{
    IMGUI_CHECKVERSION();
    d_impl->context = ImGui::CreateContext();
    d_impl->window = window;

    ImGui::StyleColorsDark(&d_impl->context->Style);

    ImGuiIO& io = d_impl->context->IO;
    
    SetBackendFlags(io);
    SetClipboardCallbacks(io, window);
    SetKeyMappings(io);
    SetFontAtlas(io, d_impl->fontAtlas);
}

void DevUI::OnEvent(Event& event)
{
    if (event.IsConsumed()) { return; }

    ImGuiIO& io = d_impl->context->IO;

    if (auto e = event.As<MouseButtonPressedEvent>()) {    
        io.MouseDown[e->Button()] = true;
    }
    
    else if (auto e = event.As<MouseButtonReleasedEvent>()) {
        io.MouseDown[e->Button()] = false;
    }

    else if (auto e = event.As<MouseMovedEvent>()) {
        io.MousePos = ImVec2(e->XPos(), e->YPos());
        if (ImGui::IsAnyWindowHovered()) { e->Consume(); }
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
    ImGuiIO& io = d_impl->context->IO;
    io.DeltaTime = (float)dt;
    io.DisplaySize = ImVec2((float)d_impl->window->Width(),
                            (float)d_impl->window->Height());
    
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
}

void DevUI::StartFrame()
{
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void DevUI::EndFrame()
{
    ImGui::SetCurrentContext(d_impl->context);
    ImGui::Render();

    Sprocket::RenderContext rc;  
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ImDrawData* drawData = &d_impl->context->DrawData;

    auto proj = Sprocket::Maths::Ortho(0, drawData->DisplaySize.x, drawData->DisplaySize.y, 0);

    d_impl->shader.Bind();
    d_impl->shader.LoadSampler("Texture", 0);
    d_impl->shader.LoadMat4("ProjMtx", proj);

    d_impl->buffer.Bind();
    d_impl->fontAtlas.Bind();

    // Render command lists
    int width = (int)drawData->DisplaySize.x;
    int height = (int)drawData->DisplaySize.y;

    for (int n = 0; n < drawData->CmdListsCount; ++n) {
        const ImDrawList* cmd_list = drawData->CmdLists[n];

        // Upload vertex/index buffers
        d_impl->buffer.SetVertexData(
            cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
            cmd_list->VtxBuffer.Data
        );
        d_impl->buffer.SetIndexData(
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

void DevUI::Gizmo(Maths::mat4* matrix,
                    const Maths::mat4& view,
                    const Maths::mat4& projection,
                    GizmoMode mode,
                    GizmoCoords coords)
{
    ImGuizmo::Manipulate(
        Maths::Cast(view),
        Maths::Cast(projection),
        GetMode(mode),
        GetCoords(coords),
        Maths::Cast(*matrix)
    );
}

}