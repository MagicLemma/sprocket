#pragma once
#include <array>
#include <string>
#include <cstddef>

namespace spkt {

class CubeMap
{
    std::uint32_t d_id;

    int d_width;
    int d_height;

public:
    CubeMap(const std::array<std::string, 6>& faceFiles);
    ~CubeMap();

    void Bind() const;
    void Unbind() const;

    int Width() const { return d_width; }
    int Height() const { return d_height; }
};

}