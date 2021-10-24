#pragma once
#include <array>
#include <string>
#include <cstddef>

namespace spkt {

class cube_map
{
    std::uint32_t d_id;

    int d_width;
    int d_height;

public:
    cube_map(const std::array<std::string, 6>& faceFiles);
    ~cube_map();

    void bind() const;
    void unbind() const;

    int width() const { return d_width; }
    int height() const { return d_height; }
};

}