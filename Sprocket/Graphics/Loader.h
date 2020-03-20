#pragma once

namespace Sprocket {
namespace Loader {

void init();
void deinit();

enum class VBOType
{
    VERTEX_BUFFER = 0,
    INDEX_BUFFER = 1
};

enum class TextureType
{
    FLAT = 0,
    CUBE = 1
};

// All of these low level loaders create the specified buffer and binds it.
// It is up to the caller of these functions to unbind them.
unsigned int createVAO();
unsigned int createVBO(VBOType type);
unsigned int createTEX(TextureType type);
unsigned int createFBO();
unsigned int createRBO();

}
}