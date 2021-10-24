#pragma once
#include <optional>

#include <glm/glm.hpp>

namespace spkt {

// When OpenGL renders to any target, it assumes the target is of the dimensions specified
// by its internal "viewport". spkt::Window guarantees that this is updated whenever the
// window is resized so that the viewport matches the screen. However, when binding a
// framebuffer or other render target, we probably want to the viewport to match the dimensions
// of the framebuffer. This class acts as an RAII wrapper for setting the viewport to a
// different size. It can also be used to set and restore the original viewport by calling
// functions manually.
class viewport
{
    std::optional<glm::ivec4> d_original;

public:
    viewport(int x, int y, int width, int height); // (x, y) lower left, width, height
    viewport(); // Does not set the viewport

    ~viewport();

    void set(int x, int y, int width, int height);
    void restore();

    // Returns the current dimensions of the viewport
    static glm::ivec4 current_viewport();

    // Returns the current aspect ratio of the viewport
    static float current_aspect_ratio();
};

}