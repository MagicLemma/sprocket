#pragma once
#include "Maths.h"
#include "Resources.h"

#include <vector>

namespace Sprocket {

class StreamBuffer 
// A class to be used with the intention of streaming data to VBOs.
// By default, the VBO has 3 attribute pointers enabled, but nothing
// is defined; that is up to the user.
{
    std::shared_ptr<VAO> d_vao;
    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;

public:
    StreamBuffer();
    ~StreamBuffer();

    void bind() const;
    void unbind() const;

    void setVertexData(std::size_t size, const void* data);
    void setIndexData(std::size_t size, const void* data);
        // Sets the data inside the StreamBuffer object. The
        // StreamBuffer object MUST be bound before calling these
        // functions, otherwise the behaviour is undefined.
};

}