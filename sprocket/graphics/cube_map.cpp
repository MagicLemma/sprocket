#include "cube_map.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <array>
#include <cstddef>
#include <string>

namespace spkt {
    
cube_map::cube_map(const std::array<std::string, 6>& faceFiles)
{
    glGenTextures(1, &d_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, d_id);

    for (std::uint32_t i = 0; i != faceFiles.size(); ++i) {
        int bpp = 0;
        unsigned char* data = stbi_load(faceFiles[i].c_str(),
            &d_width, &d_height, &bpp, 0);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGB, d_width, d_height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    // Unbind the texture now it is set up.
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

cube_map::~cube_map()
{
    glDeleteTextures(1, &d_id);
}

void cube_map::bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, d_id);
}

void cube_map::unbind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}