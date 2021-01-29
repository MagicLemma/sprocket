#pragma once
#include <optional>

#include <glm/glm.hpp>

namespace Sprocket {

// When OpenGL renders to any target, it assumes the target is of the dimensions specified
// by its internal "viewport". Sprocket::Window guarantees that this is updated whenever the
// window is resized so that the viewport matches the screen. However, when binding a
// framebuffer or other render target, we probably want to the viewport to match the dimensions
// of the framebuffer. This class acts as an RAII wrapper for setting the viewport to a
// different size. It can also be used to set and restore the original viewport by calling
// functions manually.
class Viewport
{
    std::optional<glm::ivec4> d_original;

public:
    Viewport(int x, int y, int width, int height); // (x, y) lower left, width, height
    Viewport(); // Does not set the viewport

    ~Viewport();

    void Set(int x, int y, int width, int height);
    void Restore();

    static glm::ivec4 Current(); // Returns the current dimensions of the viewport
};

}