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

    // All of the effects that will be applied to the scene image. The last effect
    // is kept separate as it is used to do the final render to the scene rather
    // than to a frame buffer.
    std::vector<spkt::shader_ptr> d_effects;
    spkt::shader_ptr d_last_effect;

public:
    post_processor(int width, int height);

    void add_effect(std::string_view vertex_shader, std::string_view fragment_shader);

    // Should be called before rendering the scene. Anything rendered between this
    // and end_frame will be written the post processor rather than the screen.
    void start_frame();

    // Ends the frame, applies all affects and prints to the screen. Anything rendered
    // after this will be rendered to the screen.
    void end_frame();

    void set_screen_size(int width, int height);
};

}