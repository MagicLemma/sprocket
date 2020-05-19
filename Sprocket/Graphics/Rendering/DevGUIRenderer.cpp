#include "DevGUIRenderer.h"
#include "RenderContext.h"

#include <imgui.h>
#include <glad/glad.h>

namespace Sprocket {
namespace {

unsigned int cast(ImTextureID id)
{
    return (unsigned int)(intptr_t)id;
}

ImTextureID cast(unsigned int id)
{
    return (ImTextureID)(intptr_t)id;
}

}

DevGUIRenderer::DevGUIRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/DevGUI.vert",
               "Resources/Shaders/DevGUI.frag")
    , d_vertexBuffer(std::make_shared<VBO>())
    , d_indexBuffer(std::make_shared<VBO>())
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    unsigned char* data;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
    d_fontAtlas = Texture(width, height, data);
    io.Fonts->TexID = cast(d_fontAtlas.id());

    GLint current_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture);
}

void DevGUIRenderer::update() const
{

}

void DevGUIRenderer::startFrame() const
{
    ImGui::NewFrame();
}

void DevGUIRenderer::endFrame() const
{
    ImGui::Render();
}

void DevGUIRenderer::draw() const
{
    Sprocket::RenderContext rc;  
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    ImDrawData* drawData = ImGui::GetDrawData();

    auto proj = Sprocket::Maths::ortho(0, drawData->DisplaySize.x, drawData->DisplaySize.y, 0);

    d_shader.bind();
    d_shader.loadUniform("Texture", 0);
    d_shader.loadUniform("ProjMtx", proj);

    Sprocket::VAO vao;
    glBindVertexArray(vao.value());
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer->value());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer->value());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          sizeof(ImDrawVert), (void*)offsetof(ImDrawVert, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          sizeof(ImDrawVert), (void*)offsetof(ImDrawVert, uv));
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                          sizeof(ImDrawVert), (void*)offsetof(ImDrawVert, col));

    // Render command lists
    int width = (int)drawData->DisplaySize.x;
    int height = (int)drawData->DisplaySize.y;

    for (int n = 0; n < drawData->CmdListsCount; ++n) {
        const ImDrawList* cmd_list = drawData->CmdLists[n];

        // Upload vertex/index buffers
        glBufferData(
            GL_ARRAY_BUFFER, 
            cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
            cmd_list->VtxBuffer.Data,
            GL_STREAM_DRAW);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
            cmd_list->IdxBuffer.Data,
            GL_STREAM_DRAW);

        for (int i = 0; i < cmd_list->CmdBuffer.Size; ++i) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[i];
            const ImVec4& rect = pcmd->ClipRect;

            if (rect.x < width && rect.y < height && rect.z >= 0 && rect.w >= 0) {

                glScissor((int)rect.x,
                          (int)(height - rect.w),
                          (int)(rect.z - rect.x),
                          (int)(rect.w - rect.y));

                glBindTexture(GL_TEXTURE_2D, cast(pcmd->TextureId));
                glDrawElements(
                    GL_TRIANGLES,
                    pcmd->ElemCount,
                    sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                    (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx))
                );
            }
        }
    }
}

}
