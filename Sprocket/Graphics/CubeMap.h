#pragma once
#include "Types.h"

#include <array>
#include <string>
#include <cstdint>

namespace spkt {

class CubeMap
{
    u32 d_id;

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