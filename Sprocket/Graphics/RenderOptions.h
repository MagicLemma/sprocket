#pragma once

namespace Sprocket {

struct RenderOptions
{
    bool faceCulling = true;
    bool wireframe = false;
    bool depthTest = true;
};

void handleRenderOptions(const RenderOptions& options);

}