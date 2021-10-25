#pragma once
#include <sprocket/graphics/frame_buffer.h>
#include <sprocket/graphics/mesh.h>
#include <sprocket/graphics/shader.h>

#include <string_view>
#include <vector>

namespace spkt {

class post_processor
{
    // The quad that is written to the frame buffer when applying the affect,
    // should mostly always match the size of the screen.
    spkt::static_mesh_ptr d_quad;

    // Post-processing uses two frame buffers. When applying an effect, we write
    // the texture from the source to the target frame buffer via the effect shader.
    spkt::frame_buffer_ptr d_target;
    spkt::frame_buffer_ptr d_source;

    std::vector<spkt::shader_ptr> d_effects;

public:
    post_processor(int width, int height);

    // Appends a post process effect to the end of the process pipeline.
    void add_effect(std::string_view vertex_shader, std::string_view fragment_shader);

    // All rendering that the effects should be applied to should be done between these
    // two function calls. end_frame will then render the result to the screen.
    void start_frame();
    void end_frame();

    // Resize the internal frame buffers.
    void set_screen_size(int width, int height);
};

}