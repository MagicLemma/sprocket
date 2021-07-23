#pragma once
#include <Sprocket/Graphics/FrameBuffer.h>
#include <Sprocket/Graphics/Mesh.h>
#include <Sprocket/Graphics/shader.h>

#include <memory>
#include <vector>

namespace spkt {

class post_processor
{
    // The quad that is written to the frame buffer when applying the affect,
    // should mostly always match the size of the screen.
    std::unique_ptr<Mesh> d_quad;

    // Post-processing uses two frame buffers, when applying an effect
    // we use one are the source and one as the destination, switching
    // each time.
    spkt::frame_buffer_ptr d_front_buffer;
    spkt::frame_buffer_ptr d_back_buffer;

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