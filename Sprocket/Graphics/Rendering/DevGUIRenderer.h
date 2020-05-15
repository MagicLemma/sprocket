#pragma once
#include "DevGUI.h"
#include "Shader.h"
#include "Window.h"
#include "Texture.h"
#include "Resources.h"

namespace Sprocket {

class DevGUIRenderer
{
    Window* d_window;
    Shader  d_shader;
    Texture d_fontAtlas;

    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;

public:
    DevGUIRenderer(Window* window);

    void update() const;

    void startFrame() const;
    void endFrame() const;

    void draw() const;
};

}