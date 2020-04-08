#pragma once
#include "Maths.h"
#include "Resources.h"

#include <vector>
#include <string>

namespace Sprocket {

struct Vertex3D
{
    Maths::vec3 position;
    Maths::vec2 texture;
    Maths::vec3 normal;
};

using Vertex3DBuffer = std::vector<Vertex3D>;
using IndexBuffer = std::vector<unsigned int>;

class Model3D
{
    // GPU STORAGE
    std::shared_ptr<VAO> d_vao;
    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;
        // The VBOs are not needed again after the attrib pointers in the
        // VAO are set. They are kept here so that this Model object can
        // manage their lifetimes; they are cleaned up when this Model
        // destructs.

    // CPU STORAGE
    Vertex3DBuffer d_vertexData;
    IndexBuffer    d_indexData;
        // If keepBuffers is true when constructing this Model3D, these
        // store the vertex and index data that has also been loaded into
        // the VBOs above. This can be used for batching. If keepBuffers is
        // false, these are empty.

    int d_vertexCount;

    std::shared_ptr<VBO> loadVertexBuffer(const Vertex3DBuffer& vertices);
    std::shared_ptr<VBO> loadIndexBuffer(const IndexBuffer& indices);

public:
    // CONSTRUCTORS
    Model3D(const std::string& objFile,
            bool storeGPU = true,
            bool storeCPU = false);

    Model3D(const Vertex3DBuffer& vertices,
            const IndexBuffer& indices,
            bool storeGPU = true,
            bool storeCPU = false);

        // If storeGPU is true, a VAO and two VBOs are created, and the data
        // is loaded in and the layouts set. These may be used by binding the
        // model.

        // If store CPU is true, the vertex buffer and index buffer vectors
        // are copied into this object and stored. These may be retreived by
        // the getter functions below. This is useful for batch rendering.

    Model3D(); // Empty model

    int vertexCount() const { return d_vertexCount; }

    // GPU STORAGE ACCESS
    void bind() const;
    void unbind() const;

    // CPU STORAGE ACCESS
    const Vertex3DBuffer& vertexBuffer() const { return d_vertexData; }
    const IndexBuffer& indexBuffer() const { return d_indexData; }
    void clear(); // Empties d_vertexData and d_indexData.
};

}