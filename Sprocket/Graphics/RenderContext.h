#pragma once
#include <glm/glm.hpp>

namespace spkt {

class Window;

class RenderContext
// A wrapper class that stores the current context and restores it upon
// destruction.
{
    bool d_cullFace;
    bool d_depthTest;
    bool d_scissorTest;

    bool d_blend;
    int  d_blendSrcAlpha;
    int  d_blendDstAlpha;
    int  d_blendEqnAlpha;
    int  d_blendSrcRGB;
    int  d_blendDstRGB;
    int  d_blendEqnRGB;

    int  d_polygonMode[2]; // Returns two values, one for front, one for back

public:
    RenderContext();
    ~RenderContext();

    void AlphaBlending(bool enabled) const;
    void FaceCulling(bool enabled) const;
    void DepthTesting(bool enabled) const;
    void ScissorTesting(bool enabled) const;
    void set_scissor_window(const glm::vec4& region) const; // Also enables scissor testing

};

}