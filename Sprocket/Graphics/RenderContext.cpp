#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {

RenderContext::RenderContext()
    : d_cullFace(glIsEnabled(GL_CULL_FACE))
    , d_depthTest(glIsEnabled(GL_DEPTH_TEST))
    , d_blend(glIsEnabled(GL_BLEND))
    , d_scissorTest(glIsEnabled(GL_SCISSOR_TEST))
    , d_blendSrcAlpha(0)
    , d_blendDstAlpha(0)
    , d_blendEqnAlpha(0)
    , d_blendSrcRGB(0)
    , d_blendDstRGB(0)
    , d_blendEqnRGB(0)
{
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &d_blendSrcAlpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &d_blendDstAlpha);
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &d_blendEqnAlpha);
    glGetIntegerv(GL_BLEND_SRC_RGB, &d_blendSrcRGB);
    glGetIntegerv(GL_BLEND_DST_RGB, &d_blendDstRGB);
    glGetIntegerv(GL_BLEND_EQUATION_RGB, &d_blendEqnRGB);
    glGetIntegerv(GL_POLYGON_MODE, d_polygonMode);
}

RenderContext::~RenderContext()
{
    if (d_cullFace) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (d_depthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (d_scissorTest) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    if (d_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    glBlendEquationSeparate(d_blendEqnRGB, d_blendEqnAlpha);
    glBlendFuncSeparate(d_blendSrcRGB, d_blendDstRGB, d_blendSrcAlpha, d_blendDstAlpha);
    glPolygonMode(GL_FRONT_AND_BACK, d_polygonMode[0]);
}

void RenderContext::AlphaBlending(bool enabled) const
{
    if (enabled) {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }
}

void RenderContext::FaceCulling(bool enabled) const
{
    if (enabled) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void RenderContext::DepthTesting(bool enabled) const
{
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void RenderContext::ScissorTesting(bool enabled) const
{
    if (enabled) {
        glEnable(GL_SCISSOR_TEST);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

}