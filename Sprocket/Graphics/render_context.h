#pragma once
#include <glm/glm.hpp>

namespace spkt {

class render_context
// A wrapper class that stores the current context and restores it upon
// destruction.
{
    bool d_depth_test;
    bool d_scissor_test;
    
    bool d_cull_face;
    int d_cull_face_mode;

    bool d_blend;
    int  d_blend_src_alpha;
    int  d_blend_dst_alpha;
    int  d_blend_equation_alpha;

    int  d_blend_src_rgb;
    int  d_blend_dst_rgb;
    int  d_blend_equation_rgb;

    int  d_polygon_mode[2]; // Returns two values, one for front, one for back

public:
    render_context();
    ~render_context();

    void alpha_blending(bool enabled) const;
    
    void face_culling(bool enabled) const;
    void set_face_cull(int mode) const;
    
    void depth_testing(bool enabled) const;

    void scissor_testing(bool enabled) const;
    void set_scissor_window(const glm::vec4& region) const; // Also enables scissor testing
    
    void wireframe(bool enabled) const;
};

}