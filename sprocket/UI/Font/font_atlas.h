#pragma once
#include <Sprocket/Graphics/texture.h>
#include <Sprocket/Utility/maths.h>

#include <glm/glm.hpp>

#include <cstdlib>
#include <memory>
#include <vector>

namespace spkt {

class font_atlas
{
    std::vector<glm::ivec3>  d_nodes;
    std::unique_ptr<texture> d_texture;

    int fit(int index, int width, int height);

public:
    font_atlas(int width, int height);

    glm::ivec4 get_region(std::size_t width, std::size_t height);
    void set_region(const glm::ivec4& region, const unsigned char* data);

    std::size_t width() const { return d_texture->width(); }
    std::size_t height() const { return d_texture->height(); }

    void bind(int slot) const { d_texture->bind(slot); }

    const texture* get_atlas() const { return d_texture.get(); }
};

}

