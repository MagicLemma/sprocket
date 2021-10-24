#include "render_context.h"

#include <sprocket/Graphics/Viewport.h>

#include <glad/glad.h>

namespace spkt {

render_context::render_context()
    : d_depth_test(glIsEnabled(GL_DEPTH_TEST))
    , d_scissor_test(glIsEnabled(GL_SCISSOR_TEST))
    , d_cull_face(glIsEnabled(GL_CULL_FACE))
    , d_blend(glIsEnabled(GL_BLEND))
    , d_blend_src_alpha(0)
    , d_blend_dst_alpha(0)
    , d_blend_equation_alpha(0)
    , d_blend_src_rgb(0)
    , d_blend_dst_rgb(0)
    , d_blend_equation_rgb(0)
{
    glGetIntegerv(GL_CULL_FACE_MODE, &d_cull_face_mode);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &d_blend_src_alpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &d_blend_dst_alpha);
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &d_blend_equation_alpha);
    glGetIntegerv(GL_BLEND_SRC_RGB, &d_blend_src_rgb);
    glGetIntegerv(GL_BLEND_DST_RGB, &d_blend_dst_rgb);
    glGetIntegerv(GL_BLEND_EQUATION_RGB, &d_blend_equation_rgb);
    glGetIntegerv(GL_POLYGON_MODE, d_polygon_mode);
}

render_context::~render_context()
{
    if (d_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (d_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (d_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    if (d_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    glBlendEquationSeparate(d_blend_equation_rgb, d_blend_equation_alpha);
    glBlendFuncSeparate(d_blend_src_rgb, d_blend_dst_rgb, d_blend_src_alpha, d_blend_dst_alpha);
    glPolygonMode(GL_FRONT_AND_BACK, d_polygon_mode[0]);
    glCullFace(d_cull_face_mode);
}

void render_context::alpha_blending(bool enabled) const
{
    if (enabled) {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }
}

void render_context::face_culling(bool enabled) const
{
    if (enabled) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void render_context::set_face_cull(int mode) const
{
    glEnable(GL_CULL_FACE);
    glCullFace(mode);
}

void render_context::depth_testing(bool enabled) const
{
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void render_context::scissor_testing(bool enabled) const
{
    if (enabled) {
        glEnable(GL_SCISSOR_TEST);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

void render_context::set_scissor_window(const glm::vec4& region) const
{
    glEnable(GL_SCISSOR_TEST);
    float height = spkt::viewport::current_viewport().a;
    glScissor(region.x, height - region.y - region.w, region.z, region.w);
}

void render_context::wireframe(bool enabled) const
{
    if (enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, d_polygon_mode[0]);
    }
}

}