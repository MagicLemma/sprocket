#pragma once
#include "Maths.h"
#include "Window.h"

#include <optional>

namespace Sprocket {

class RenderContext
// A wrapper class that stores the current context and restores it upon
// destruction.
{
    bool d_cullFace;
    bool d_depthTest;
    bool d_scissorTest;  // TODO: Store old scissor box to restore

    bool d_blend;
    int  d_blendSrcAlpha;
    int  d_blendDstAlpha;
    int  d_blendEqnAlpha;
    int  d_blendSrcRGB;
    int  d_blendDstRGB;
    int  d_blendEqnRGB;

    int  d_polygonMode[2];  // Segfaults if not an array, dont know why

public:
    RenderContext();
    ~RenderContext();

    void AlphaBlending(bool enabled) const;
    void FaceCulling(bool enabled) const;
    void DepthTesting(bool enabled) const;
    void ScissorTesting(bool enabled) const;

};

class ScissorContext
// An RAII class for enabling scissor testing. Scissor testing is enabled on creation
// and disabled on destruction. The window is needed here since OpenGL puts (0, 0) in the
// lower left while Sprocket has (0, 0) as the top left, so the region needs to get
// flipped in the y-axis before using.
{
    std::optional<glm::vec4> d_region;

public:
    ScissorContext(Window* window, const std::optional<glm::vec4>& region);
    ~ScissorContext();
};

}