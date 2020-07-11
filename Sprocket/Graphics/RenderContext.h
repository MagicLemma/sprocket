#pragma once

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

};

}