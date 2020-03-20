#pragma once
#include <array>

namespace Sprocket {

class Skybox
{
    unsigned int d_vaoId;
    unsigned int d_texId;

public:
    Skybox(const std::array<std::string, 6>& faceFiles);

    void bind() const;
    void unbind() const;

    // Getters / Setters
    unsigned int model() const { return d_vaoId; }
    unsigned int texture() const { return d_texId; }
};

}